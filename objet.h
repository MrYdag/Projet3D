//
// Created by Emanuel on 10/01/2021.
//

#ifndef OBJET_H
#define OBJET_H

#include <vector>
#include "geometry.h"
#include "tgaimage.h"

class Objet {
private:
    std::vector<Vec3f> verts_; //liste des sommets
    std::vector<std::vector<int>> faces_; //liste des triangles
    std::vector<Vec3f> texture; //texture de l'image
    std::vector<std::vector<int>> faces_texture; //liste des textures
    
    std::vector<Vec3f> vecnorm; //texture de l'image
    std::vector<std::vector<int>> faces_norm; //liste des vecteurs normaux
    std::vector<std::vector<int>> faces_norm_map; //liste des vecteur normaux avec la map
    
    TGAImage normalmap_;
    
public:
    Objet(const char *filename);
    int nverts();
    int nfaces();
    Vec3f getVertice(int i);
    std::vector<int> getFace(int i);
    std::vector<int> getFaceTexture(int i);
    Vec3f getTexture(int i);
    Vec3f norm(int iface, int nvert);
    Vec3f normmap(int iface, int nvert);
    
    Vec3f normal(Vec2f uvf);

};

#endif
