#pragma once

#include <string>
#include <vector>

#include "CubeMesh.hpp"
#include "Vertex.hpp"

class Loader {
public:
    Loader() {}
    ~Loader();

    std::vector<unsigned int> GetIndices();
    std::vector<CubeMesh> GetMeshes();
    std::vector<Vertex> GetVertices();
    bool ParseFile(const std::string& objFilePath);

private:
    void GenerateVerticesFromRawObj(std::vector<Vertex>& vertices, const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& coordinates, const std::vector<glm::vec3>& normals, std::string currentLine);
    void TriangulateVertices(std::vector<unsigned int>& indices, const std::vector<Vertex>& vertices);

    std::vector<unsigned int> m_indices;
    std::vector<CubeMesh> m_meshes;
    std::vector<Vertex> m_vertices;
};