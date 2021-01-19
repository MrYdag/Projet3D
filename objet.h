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
public:
    Objet(const char *filename);
    int nverts();
    int nfaces();
    Vec3f getVertice(int i);
    std::vector<int> getFace(int i);

};

#endif
