#pragma once

#include "GLM/vec3.hpp"

#include "Light.hpp"

struct DirectionalLight : Light {
public:
    glm::vec3 Direction;
};