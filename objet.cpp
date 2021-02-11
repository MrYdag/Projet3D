//
// Created by Emanuel on 10/01/2021.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "objet.h"

Objet::Objet(const char *filename) {
    std::ifstream in;
    in.open(filename,std::ifstream::in);
    if(in.fail()) {
        return;
    }
    std::string line;
    while(!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) { //Ligne qui commence par v: les sommets
            iss >> trash;
            Vec3f v;
            for (int i = 0; i < 3; i++) {
                iss >> v[i];
            }
            verts_.push_back(v);
        }
        if (!line.compare(0, 2, "f ")) { //Ligne qui commence par f: les triangles
            std::vector<int>f;
            std::vector<int>f2;
            int itrash, idx, idx2;
            iss >> trash;
            while (iss >> idx >> trash >> idx2 >> trash >> itrash) {
                idx--; // in wavefront obj all indices start at 1, not zero
                idx2--;
                f.push_back(idx);
                f2.push_back(idx2);
            }
            faces_.push_back(f);
            faces_texture.push_back(f2);
        }
        if (!line.compare(0, 3, "vt ")) { //Ligne qui commence par vt: les textures
            iss >> trash;
            iss >> trash;
            Vec3f vt;
            for (int i = 0; i < 3; i++) {
                iss >> vt[i];
            }
            texture.push_back(vt);
        }
    }
}

Vec3f Objet::getVertice(int i) {
    return verts_[i];
}


int Objet::nverts() {
    return (int)verts_.size();
}

std::vector<int> Objet::getFace(int i) {
    return faces_[i];
}

int Objet::nfaces() {
    return (int)faces_.size();
}

std::vector<int> Objet::getFaceTexture(int i) {
    return faces_texture[i];
}

Vec3f Objet::getTexture(int i) {
    return texture[i];
}

Vec3f Objet::norm(int iface, int nvert) {
    int idx = faces_[iface][nvert];
    return norms_[idx].normalize();
}



