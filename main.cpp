#include <cmath>
#include "tgaimage.h"
#include "objet.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor blue   = TGAColor( 0, 0,   255,   255);
const TGAColor green   = TGAColor(0, 255,   0,   255);

const int width = 800;
const int height = 800;

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




    /*
    line(p1.x,p1.y,p2.x,p2.y, image,color);
    line(p3.x,p3.y,p2.x,p2.y, image,color);
    line(p1.x,p1.y,p3.x,p3.y, image,color);
     */
}


int main(int argc, char** argv) {
    TGAImage image(width, height, TGAImage::RGB);

    Objet *objet = new Objet("obj/head.obj");
    Vec3f light_dir(0,0,-1);
    for(int i = 0; i< objet->nfaces(); i++){
        std::vector<int> list = objet->getFace(i);

        Vec3f v1 = objet->getVertice(list[0]);
        Vec3f v2 = objet->getVertice(list[1]);
        Vec3f v3 = objet->getVertice(list[2]);
        int x0 = (v1.x+1.)*width/2.;
        int y0 = (v1.y+1.)*height/2.;
        int x1 = (v2.x+1.)*width/2.;
        int y1 = (v2.y+1.)*height/2.;
        int x2 = (v3.x+1.)*width/2.;
        int y2 = (v3.y+1.)*height/2.;

        //Calcul de la lumiere
        Vec3f n = (v3-v1)^(v2-v1);
        n.normalize();
        float intensity = n*light_dir;
        if(intensity > 0){
            triangle(Vec2i(x0, y0), Vec2i(x1, y1), Vec2i(x2, y2),image,TGAColor(intensity*255, intensity*255, intensity*255, 255));
        }
    }
    image.write_tga_file("output.tga");

    return 0;
}