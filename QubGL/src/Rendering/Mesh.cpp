#include "Mesh.hpp"

#include <iostream>

#include "ElementBufferObject.hpp"
#include "Material.hpp"
#include "Vertex.hpp"
#include "VertexArrayObject.hpp"
#include "VertexBufferObject.hpp"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
    :m_ebo(nullptr), m_vao(nullptr), m_vbo(nullptr) {
    if (!m_isLoaded) {
        Load(&vertices, &indices);
    }
}

void Mesh::Bind() const {
    m_vao->Bind();
    m_vbo->Bind();
    m_ebo->Bind();
}

void Mesh::Draw() const {
    Bind();

    glDrawElements(GL_TRIANGLES, m_ebo->GetIndicesCount(), GL_UNSIGNED_INT, (void*)0);

    Unbind();
}

Material Mesh::GetMaterial() const {
    return m_material;
}

std::string Mesh::GetName() const {
    return m_name;
}

void Mesh::Load(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices) {
    m_vao = new VertexArrayObject();
    m_vbo = new VertexBufferObject();
    m_ebo = new ElementBufferObject();

    m_vao->Bind();
    m_vbo->Bind();

    m_vbo->BufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(Vertex), &vertices->front(), GL_STATIC_DRAW);
    m_vao->SetupAttribPointers();

    m_vbo->Unbind();
    m_ebo->Bind();

    m_ebo->BufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(unsigned int), &indices->front(), GL_STATIC_DRAW);

    m_ebo->Unbind();
    m_vao->Unbind();

    m_isLoaded = true;
}

void Mesh::SetMaterial(Material material) {
    m_material = material;
}

void Mesh::SetName(std::string name) {
    m_name = name;
}

void Mesh::Unbind() const {
    m_vao->Unbind();
    m_vbo->Unbind();
    m_ebo->Unbind();
}

void Mesh::Unload() {
    delete m_vao;
    delete m_vbo;
    delete m_ebo;

    m_isLoaded = false;
}