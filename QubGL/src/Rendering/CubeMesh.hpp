#pragma once

#include <string>
#include <vector>

struct Vertex;

class CubeMesh {
public:
    CubeMesh() : m_ebo(nullptr), m_vao(nullptr), m_vbo(nullptr) { }
    CubeMesh(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices);

    void Bind() const;
    void Draw() const;
    std::string GetName() const;
    void SetName(std::string name);
    void Unbind() const;

private:
    void Load(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices);
    void Unload();

    class ElementBufferObject* m_ebo;
    bool m_isLoaded = false;
    std::string m_name = "unnamed";
    class VertexArrayObject* m_vao;
    class VertexBufferObject* m_vbo;
};