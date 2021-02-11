# cmake_minimum_required(VERSION <specify CMake version here>)
project(Projet3D)

set(CMAKE_CXX_STANDARD 14)

add_executable(Projet3D tgaimage.cpp tgaimage.h objet.cpp objet.h main.cpp geometry.cpp)