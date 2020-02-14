#include "CubeMesh.hpp"

#include "ElementBufferObject.hpp"
#include "VertexArrayObject.hpp"
#include "VertexBufferObject.hpp"

CubeMesh::CubeMesh(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices)
    :m_ebo(nullptr), m_vao(nullptr), m_vbo(nullptr) {
    if (!m_isLoaded) {
        Load(vertices, indices);
    }
}

void CubeMesh::Bind() const {
    m_ebo->Bind();
    m_vao->Bind();
    m_vbo->Bind();
}

void CubeMesh::Draw() const {
    Bind();

    glDrawElements(GL_TRIANGLES, m_ebo->GetIndicesCount(), GL_UNSIGNED_INT, (void*)0);

    Unbind();
}

std::string CubeMesh::GetName() const {
    return m_name;
}

void CubeMesh::Load(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices) {
    m_ebo = new ElementBufferObject();
    m_vao = new VertexArrayObject();
    m_vbo = new VertexBufferObject();

    m_vao->Bind();
    m_vbo->Bind();

    m_vbo->BufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    m_vao->SetupAttribPointers();

    m_vbo->Unbind();
    m_ebo->Bind();

    m_ebo->BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    m_ebo->Unbind();
    m_vao->Unbind();

    m_isLoaded = true;
}

void CubeMesh::SetName(std::string name) {
    m_name = name;
}

void CubeMesh::Unbind() const {
    m_ebo->Unbind();
    m_vao->Unbind();
    m_vbo->Unbind();
}

void CubeMesh::Unload() {
    delete m_ebo;
    delete m_vao;

    m_isLoaded = false;
}