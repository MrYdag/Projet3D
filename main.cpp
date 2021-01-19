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
    line(p1.x,p1.y,p2.x,p2.y, image,color);
    line(p3.x,p3.y,p2.x,p2.y, image,color);
    line(p1.x,p1.y,p3.x,p3.y, image,color);
}


int main(int argc, char** argv) {
    TGAImage image(width, height, TGAImage::RGB);

    Objet *objet = new Objet("obj/head.obj");
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
        triangle(Vec2i(x0, y0), Vec2i(x1, y1), Vec2i(x2, y2),image,white);
    }
    image.write_tga_file("output.tga");

    return 0;
}