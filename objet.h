//
// Created by Emanuel on 10/01/2021.
//

#ifndef OBJET_H
#define OBJET_H

#include <vector>
#include "geometry.h"

class Objet {
private:
    std::vector<Vec3f> verts_; //liste des sommets
    std::vector<std::vector<int>> faces_; //liste des triangles
    std::vector<Vec3f> texture; //texture de l'image
    std::vector<std::vector<int>> faces_texture; //liste des textures
public:
    Objet(const char *filename);
    int nverts();
    int nfaces();
    Vec3f getVertice(int i);
    std::vector<int> getFace(int i);
    std::vector<int> getFaceTexture(int i);
    Vec3f getTexture(int i);

};

#endif
