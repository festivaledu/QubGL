//
//  Vertexhpp
//  QubGL
//
//  Copyright © 2020 Team FESTIVAL. All rights reserved.
//

#pragma once

#include "GLM\vec2.hpp"
#include "GLM\vec3.hpp"
#include "GLM\vec4.hpp"

struct Vertex {
public:
    Vertex(glm::vec3 position = glm::vec3(0.F, 0.F, 0.F), glm::vec4 color = glm::vec4(1.F, 1.F, 1.F, 1.F), glm::vec3 normal = glm::vec3(0.F, 0.F, 0.F));

    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec3 Normal;
    glm::vec2 TextureCoordinates = glm::vec2(0.F);
};