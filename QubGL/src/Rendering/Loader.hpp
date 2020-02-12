#pragma once

#include "GLM/vec3.hpp"
#include "GLM/vec4.hpp"

#include <string>
#include <vector>

class Mesh;
class Vertex;

class Loader {
public:
	Loader() {}
	~Loader();
	
	std::vector<unsigned int> GetIndices();
	std::vector<Mesh> GetMeshes();
	std::vector<Vertex> GetVertices();
	bool ParseFile(const std::string& objFilePath);

private:
	void GenerateVerticesFromRawObj(std::vector<Vertex>& vertices, const std::vector<glm::vec3>& positions, const std::vector<glm::vec4>& coordinates, const std::vector<glm::vec3>& normals, std::string currentLine);
	void TriangulateVertices(std::vector<unsigned int>& indices, const std::vector<Vertex>& vertices);

	std::vector<unsigned int> m_indices;
	std::vector<Mesh> m_meshes;
	std::vector<Vertex> m_vertices;
};