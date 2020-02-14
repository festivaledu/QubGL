#pragma once

#include "GLM/vec4.hpp"

struct Light {
public:
    glm::vec4 Ambient;
    glm::vec4 Diffuse;
    glm::vec4 Specular;
};