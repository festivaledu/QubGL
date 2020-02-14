#include "Model.hpp"

#include "CubeMesh.hpp"
#include "ShaderProgram.hpp"

Model::Model(const CubeMesh& mesh)
    :m_mesh(mesh) { }

Model::~Model() { }

void Model::Draw(const ShaderProgram& shader) {
    shader.Bind();
    shader.SetModelMatrix(*this);
    m_mesh.Draw();
}

Transform& Model::GetTransform() {
    return m_transform;
}