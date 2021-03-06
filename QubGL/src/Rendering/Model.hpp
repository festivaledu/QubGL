//
//  Model.hpp
//  QubGL
//
//  Copyright © 2020 Team FESTIVAL. All rights reserved.
//

#pragma once

#include "GLM/vec3.hpp"

#include <vector>

#include "Transform.hpp"

class Model {
public:
    Model(const class Mesh& mesh);
    ~Model();

    void Draw(const class ShaderProgram& shader);
    Transform& GetTransform();

    glm::vec3 Colors;
    glm::vec3 Position;

private:
    const class Mesh& m_mesh;
    Transform m_transform;
};