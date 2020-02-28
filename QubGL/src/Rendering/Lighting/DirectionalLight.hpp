//
//  DirectionalLight.hpp
//  QubGL
//
//  Copyright © 2020 Team FESTIVAL. All rights reserved.
//

#pragma once

#include "GLM/vec3.hpp"

#include "Light.hpp"

struct DirectionalLight : Light {
public:
    glm::vec3 Direction;
};