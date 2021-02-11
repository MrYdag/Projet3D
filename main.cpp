#include <cmath>
#include "tgaimage.h"
#include "objet.h"
#include <limits>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor blue   = TGAColor( 0, 0,   255,   255);
const TGAColor green   = TGAColor(0, 255,   0,   255);

const int width = 800;
const int height = 800;
const int depth = 255;
float *zbuffer = new float [width*height];

Vec3f light_dir = Vec3f(0,2,-5).normalize(); //todo changer le produit de matrice pour que les coordonnées soient positives
Vec3f camera(1,1,3);
Vec3f center(0,0,0);

//test
TGAImage texture;

///////////////////////

Vec3i m2v(Matrix m) {
    float rapport = m[3][0];
    return Vec3i(m[0][0]/rapport, m[1][0]/rapport, m[2][0]/rapport);
}
/**
 * Ceci est extremement moche, mais vu qu'on ne peux pas cast un Vec3f en Vec3i ...
 * @param m
 * @param a
 * @return
 */
Vec3f m2v(Matrix m, std::string a) {
    float rapport = m[3][0];
    return Vec3f(m[0][0]/rapport, m[1][0]/rapport, m[2][0]/rapport);
}

Matrix v2m(Vec3f v) {
    Matrix m(4, 1);
    m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
    m[3][0] = 1.f;
    return m;
}

Matrix viewport(int x, int y, int w, int h) {
    Matrix m = Matrix::identity(4);
    m[0][3] = x+w/2.f;
    m[1][3] = y+h/2.f;
    m[2][3] = depth/2.f;

    m[0][0] = w/2.f;
    m[1][1] = h/2.f;
    m[2][2] = depth/2.f;
    return m;
}

Matrix lookat(Vec3f eye, Vec3f center, Vec3f up) {
    Vec3f z = (eye-center).normalize();
    Vec3f x = cross(up,z).normalize();
    Vec3f y = cross(z,x).normalize();
    Matrix res = Matrix::identity(4);
    for (int i=0; i<3; i++) {
        res[0][i] = x[i];
        res[1][i] = y[i];
        res[2][i] = z[i];
        res[i][3] = -center[i];
    }
    return res;
}
/////////////////////////



void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {

    bool steep = false;
    if (std::abs(x0-x1)<std::abs(y0-y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0>x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1-x0;
    int dy = y1-y0;
    int derror2 = std::abs(dy)*2;
    int error2 = 0;
    int y = y0;
    for (int x=x0; x<=x1; x++) {
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
        error2 += derror2;

        if (error2>dx) {
            y += (y1>y0?1:-1);
            error2 -= dx*2;
        }
    }
}


Vec3f barycentric(Vec3i p1, Vec3i p2, Vec3i p3, Vec3f P) {
    Vec3f u = cross(Vec3f(p3.x-p1.x, p2.x-p1.x, p1.x-P.x), Vec3f(p3.y-p1.y, p2.y-p1.y,p1.y-P.y));
    if (std::abs(u[2])<1) return Vec3f(-1,1,1);
    return Vec3f(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
}

//Triangle en coupant le triangle en deux et en coloriant les deux parties
/*
void triangle(Vec2i p1, Vec2i p2, Vec2i p3, TGAImage &image, TGAColor color){
    //On prend en compte le cas où nous avont un triangle plat
    if (p1.y == p2.y && p1.y == p3.y) return;
    //On trie les points de haut en bas
    if(p1.y > p2.y) std::swap(p1,p2);
    if(p1.y > p3.y) std::swap(p1,p3);
    if(p2.y > p3.y) std::swap(p2,p3);
    //Calcul la distance entre le point le plus haut et le point le plus bas
    int total_height = p3.y-p1.y;
    //Colorie la partie basse du triangle
    for(int y = p1.y; y < p2.y; y++ ){
        int segment_height = p2.y-p1.y+1;
        float alpha = (float)(y-p1.y)/total_height;
        float beta = (float)(y-p1.y)/segment_height;
        Vec2i A = p1 + (p3-p1)*alpha;
        Vec2i B = p1 + (p2-p1)*beta;
        if(A.x>B.x) std::swap(A,B);
        for(int j=A.x; j<=B.x;j++){
            image.set(j,y,color);
        }
    }
    //Colorie le reste du triangle
    for(int y = p2.y; y < p3.y; y++ ){
        int segment_height = p3.y-p2.y+1;
        float alpha = (float)(y-p1.y)/total_height;
        float beta = (float)(y-p2.y)/segment_height;
        Vec2i A = p1 + (p3 - p1) * alpha;
        Vec2i B = p2 + (p3-p2) * beta;
        if(A.x>B.x) std::swap(A,B);
        for(int j=A.x; j<=B.x; j++){
            image.set(j,y,color);
        }
    }
}
*/

/**
 *
 * @param p1 Point1
 * @param p2 Point2
 * @param p3 Point3
 * @param zbuffer
 * @param image
 * @param color couleur du triangle
 */

void triangle(Vec3i p1,Vec3i p2,Vec3i p3,float *zbuffer, Vec3f ptsTexture[3],TGAImage &image, float intensity) {

    Vec3i pts[3] = {p1,p2,p3};

    Vec2f bboxmin( std::numeric_limits<float>::max(),  std::numeric_limits<float>::max());
    Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    Vec2f clamp(image.get_width()-1, image.get_height()-1);
    for (int i=0; i<3; i++) {
        for (int j=0; j<2; j++) {
            bboxmin[j] = std::max(0.f,        std::min(bboxmin[j], float (pts[i][j])));
            bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], float (pts[i][j])));
        }
    }
    Vec3f P;
    for (P.x=bboxmin.x; P.x<=bboxmax.x; P.x++) {
        for (P.y=bboxmin.y; P.y<=bboxmax.y; P.y++) {
            Vec3f bc_screen  = barycentric(p1,p2,p3, P);

            if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue;
            P.z = 0;
            for (int i=0; i<3; i++) P.z += pts[i][2]*bc_screen[i];
            if (zbuffer[int(P.x+P.y*width)]<P.z) {
                zbuffer[int(P.x+P.y*width)] = P.z;

                double u = bc_screen[0]*ptsTexture[0].x + bc_screen[1]*ptsTexture[1].x + bc_screen[2]*ptsTexture[2].x;
                double v = bc_screen[0]*ptsTexture[0].y + bc_screen[1]*ptsTexture[1].y + bc_screen[2]*ptsTexture[2].y;

                TGAColor color = texture.get(u*texture.get_width(),v*texture.get_height());
                //TGAColor color = TGAColor(255,255,255);
                image.set(P.x, P.y, color*intensity);
            }
        }
    }
}


int main(int argc, char** argv) {
    TGAImage image(width, height, TGAImage::RGB);

    Matrix ModelView = lookat(camera,center,Vec3f(0,1,0));

    Matrix Projection = Matrix::identity(4);
    Matrix ViewPort   = viewport(width/8, height/8, width*3/4, height*3/4);
    Projection[3][2] = -1.f/(camera-center).norm();

    texture.read_tga_file("obj/head.tga");
    texture.flip_vertically();

    Objet *objet = new Objet("obj/head.obj");


    for (int i=width*height; i--;zbuffer[i] = -std::numeric_limits<float>::max());


    for(int i = 0; i< objet->nfaces(); i++){
        std::vector<int> list = objet->getFace(i);
        std::vector<int> listTexture = objet->getFaceTexture(i);

        Vec3f v[3];

        Vec3f ptsTexture[3];

        Vec3i screen_coords[3];
        Vec3f screen_coords_f[3];
        //float intensity[3];
        for (int j = 0; j<3;j++){
            v[j] = objet->getVertice(list[j]);

            screen_coords[j] = m2v(ViewPort*Projection*ModelView*v2m(v[j]));
            screen_coords_f[j] = m2v(ViewPort*Projection*v2m(v[j]),"");

            ptsTexture[j] = objet->getTexture(listTexture[j]);

        }

        //Calcul de la lumiere
        Vec3f n = cross ((screen_coords_f[2]-screen_coords_f[0]), (screen_coords_f[1]-screen_coords_f[0]));
        n.normalize();
        float intensity = n *light_dir;

            triangle(screen_coords[0], screen_coords[1], screen_coords[2],zbuffer, ptsTexture,image,intensity);
    }
    image.write_tga_file("output.tga");

    //Affichage du Zbuffer
    float zmin = +std::numeric_limits<float>::max();
    float zmax = -std::numeric_limits<float>::max();
    for (int i=width*height; i--;) {
        if (zbuffer[i]!=-std::numeric_limits<float>::max())
            zmin = std::min(zmin, zbuffer[i]);
        zmax = std::max(zmax, zbuffer[i]);
    }
    std::cerr << zmin << " " << zmax << std::endl;

    TGAImage zimg(width, height, TGAImage::GRAYSCALE);
    for (int i=width*height; i--;) {
        zimg.set(i%width, i/width, TGAColor(255*((zbuffer[i]-zmin)/(zmax-zmin))));
    }
    zimg.write_tga_file("zbuffer.tga");
    //
    delete objet;
    delete [] zbuffer;
    return 0;
}
