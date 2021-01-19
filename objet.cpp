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
                iss >> v.raw[i];
            }
            verts_.push_back(v);
        }
        if (!line.compare(0, 2, "f ")) { //Ligne qui commence par f: les triangles
            std::vector<int>f;
            int itrash, idx;
            iss >> trash;
            while (iss >> idx >> trash >> itrash >> trash >> itrash) {
                idx--; // in wavefront obj all indices start at 1, not zero
                f.push_back(idx);
            }
            faces_.push_back(f);
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


