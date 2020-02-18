#pragma once

#include "GLM/vec3.hpp"

#include <string>

struct Material {
    Material() {
        D = 0.F;
        Illum = 0;
        Name;
        Ni = 0.F;
        Ns = 0.F;
    }

    float D;
    int Illum;
    glm::vec3 Ka;
    glm::vec3 Kd;
    glm::vec3 Ks;
    std::string MapBump;
    std::string MapD;
    std::string MapKa;
    std::string MapKd;
    std::string MapKs;
    std::string MapNs;
    std::string Name;
    float Ni;
    float Ns;
};