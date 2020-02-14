#pragma once

#include "Transform.hpp"

class Model {
public:
    Model(const class CubeMesh& mesh);
    ~Model();

    void Draw(const class ShaderProgram& shader);
    Transform& GetTransform();

private:
    const class CubeMesh& m_mesh;
    Transform m_transform;
};