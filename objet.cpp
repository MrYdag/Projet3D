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
        if (!line.compare(0, 2, "v ")) { //Ligne qui commence par v
            iss >> trash;
            Vec3f v;
            for (int i = 0; i < 3; i++) {
                iss >> v.raw[i];
            }
            verts_.push_back(v);
        }
    }
}

Vec3f Objet::getVertice(int i) {
    return verts_[i];
}


int Objet::nverts() {
    return (int)verts_.size();
}


