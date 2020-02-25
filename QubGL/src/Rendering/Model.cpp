#include "Model.hpp"

#include "Mesh.hpp"
#include "ShaderProgram.hpp"

Model::Model(const Mesh& mesh)
    :m_mesh(mesh) {
    Colors = glm::vec3(0.F);
    Position = glm::vec3(0.F);
}

Model::~Model() { }

void Model::Draw(const ShaderProgram& shader) {
    shader.Bind();
    shader.SetModelMatrix(*this);
    m_mesh.Draw();
}

Transform& Model::GetTransform() {
    return m_transform;
}