#pragma once

#include <string>
#include <vector>

#include "Material.hpp"
#include "Mesh.hpp"
#include "Vertex.hpp"

class Loader {
public:
    Loader() {}
    ~Loader();

    bool LoadMaterials(const std::string& mtlFilePath);
    bool LoadObjects(const std::string& objFilePath);

    std::vector<unsigned int> Indices;
    std::vector<Material> Materials;
    std::vector<Mesh> Meshes;
    std::vector<Vertex> Vertices;

private:
    void GenerateVerticesFromRawObj(std::vector<Vertex>& vertices, const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& coordinates, const std::vector<glm::vec3>& normals, std::string currentLine);
    void TriangulateVertices(std::vector<unsigned int>& indices, const std::vector<Vertex>& vertices);
};