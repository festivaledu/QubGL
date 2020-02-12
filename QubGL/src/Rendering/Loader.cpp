#define _USE_MATH_DEFINES

#include "Loader.hpp"

#include "GLM/gtx/matrix_cross_product.hpp"
#include "GLM/gtx/vector_angle.hpp"

#include <cmath>
#include <fstream>

#include "CubeMesh.hpp"
#include "LoaderHelpers.hpp"
#include "Vertex.hpp"

using namespace std;

Loader::~Loader() {
	m_meshes.clear();
}

vector<unsigned int> Loader::GetIndices() {
	return m_indices;
}

vector<Mesh> Loader::GetMeshes() {
	return m_meshes;
}

vector<Vertex> Loader::GetVertices() {
	return m_vertices;
}

void Loader::GenerateVerticesFromRawObj(vector<Vertex>& vertices, const vector<glm::vec3>& positions, const vector<glm::vec4>& coordinates, const vector<glm::vec3>& normals, string currentLine) {
	vector<string> face, sVertices;
	Vertex vertex;

	objhelpers::split(objhelpers::tail(currentLine), face, " ");

	auto noNormal = false;

	for (auto i = 0; i < int(face.size()); i++) {
		objhelpers::VertexType vertexType;

		objhelpers::split(face[i], sVertices, "/");

		if (sVertices.size() == 1) {
			vertexType = objhelpers::Position;
		} else if (sVertices.size() == 2) {
			vertexType = objhelpers::PositionAndTexture;
		} else if (sVertices.size() == 3) {
			if (sVertices[1] == "") {
				vertexType = objhelpers::PositionAndNormal;
			} else {
				vertexType = objhelpers::PositionAndTextureAndNormal;
			}
		}

		switch (vertexType) {
		case objhelpers::Position: {
			vertex.Position = objhelpers::getElement(positions, sVertices[0]);
			vertex.Color = glm::vec4(0.F);
			noNormal = true;
			vertices.push_back(vertex);
			break;
		}
		case objhelpers::PositionAndTexture: {
			vertex.Position = objhelpers::getElement(positions, sVertices[0]);
			vertex.Color = objhelpers::getElement(coordinates, sVertices[1]);
			noNormal = true;
			vertices.push_back(vertex);
			break;
		}
		case objhelpers::PositionAndNormal: {
			vertex.Position = objhelpers::getElement(positions, sVertices[0]);
			vertex.Color = glm::vec4(0.F);
			vertex.Normal = objhelpers::getElement(normals, sVertices[2]);
			vertices.push_back(vertex);
			break;
		}
		case objhelpers::PositionAndTextureAndNormal: {
			vertex.Position = objhelpers::getElement(positions, sVertices[0]);
			vertex.Color = objhelpers::getElement(coordinates, sVertices[1]);
			vertex.Normal = objhelpers::getElement(normals, sVertices[2]);
			vertices.push_back(vertex);
			break;
		}
		default: break;
		}
	}

	if (noNormal) {
		auto a = vertices[0].Position - vertices[1].Position;
		auto b = vertices[2].Position - vertices[1].Position;

		auto normal = glm::cross(a, b);

		for (auto i = 0; i < int(vertices.size()); i++) {
			vertices[i].Normal = normal;
		}
	}
}

bool Loader::ParseFile(const string& objFilePath) {
	if (objFilePath.substr(objFilePath.size() - 4, 4) != ".obj") return false;

	ifstream stream(objFilePath);

	if (!stream.is_open()) return false;

	m_indices.clear();
	m_meshes.clear();
	m_vertices.clear();

	vector<glm::vec3> positions;
	vector<glm::vec4> coordinates;
	vector<glm::vec3> normals;

	vector<Vertex> vertices;
	vector<unsigned int> indices;

	auto isListening = false;

	string meshName;
	CubeMesh tempMesh;

	string currentLine;
	while (getline(stream, currentLine)) {
		if (objhelpers::firstToken(currentLine) == "o" || objhelpers::firstToken(currentLine) == "g" || currentLine[0] == 'g') {
			if (!isListening) {
				isListening = true;
			} else {
				if (!indices.empty() && !vertices.empty()) {
					tempMesh = CubeMesh(&vertices, &indices);
					tempMesh.SetName(meshName);

					m_meshes.push_back(tempMesh);

					vertices.clear();
					indices.clear();
					meshName.clear();

					meshName = objhelpers::tail(currentLine);
				}
			}

			if (objhelpers::firstToken(currentLine) == "o" || objhelpers::firstToken(currentLine) == "g") {
				meshName = objhelpers::tail(currentLine);
			} else {
				meshName = "unnamed";
			}
		}

		if (objhelpers::firstToken(currentLine) == "v") {
			vector<std::string> sPosition;
			glm::vec3 vPosition;

			objhelpers::split(objhelpers::tail(currentLine), sPosition, " ");

			vPosition.x = stof(sPosition[0]);
			vPosition.y = stof(sPosition[1]);
			vPosition.z = stof(sPosition[2]);

			positions.push_back(vPosition);
		}

		if (objhelpers::firstToken(currentLine) == "vt") {
			vector<string> sTexture;
			glm::vec4 vTexture;

			objhelpers::split(objhelpers::tail(currentLine), sTexture, " ");

			vTexture.r = stof(sTexture[0]);
			vTexture.g = stof(sTexture[1]);
			vTexture.b = stof(sTexture[2]);
			vTexture.a = stof(sTexture[3]);

			coordinates.push_back(vTexture);
		}

		if (objhelpers::firstToken(currentLine) == "vn") {
			vector<string> sNormal;
			glm::vec3 vNormal;

			objhelpers::split(objhelpers::tail(currentLine), sNormal, " ");

			vNormal.x = stof(sNormal[0]);
			vNormal.y = stof(sNormal[1]);
			vNormal.z = stof(sNormal[2]);

			normals.push_back(vNormal);
		}

		if (objhelpers::firstToken(currentLine) == "f") {
			vector<Vertex> vVertices;
			GenerateVerticesFromRawObj(vVertices, positions, coordinates, normals, currentLine);

			for (auto i = 0; i < int(vVertices.size()); i++) {
				vertices.push_back(vVertices[i]);
				m_vertices.push_back(vVertices[i]);
			}

			vector<unsigned int> vIndices;
			TriangulateVertices(vIndices, vVertices);

			for (auto i = 0; i < int(vIndices.size()); i++) {
				auto ii = (unsigned int)((vertices.size() - vVertices.size() + vIndices[i]));
				indices.push_back(ii);

				ii = (unsigned int)((m_vertices.size() - vVertices.size() + vIndices[i]));
				m_indices.push_back(ii);
			}
		}

		if (objhelpers::firstToken(currentLine) == "usemtl") {

		}

		if (objhelpers::firstToken(currentLine) == "mtllib") {

		}
	}

	if (!indices.empty() && !vertices.empty()) {
		tempMesh = CubeMesh(&vertices, &indices);
		tempMesh.SetName(meshName);

		m_meshes.push_back(tempMesh);
	}

	stream.close();

	return m_indices.empty() && m_meshes.empty() && m_vertices.empty();
}

void Loader::TriangulateVertices(vector<unsigned int>& indices, const vector<Vertex>& vertices) {
	if (vertices.size() < 3) return;

	if (vertices.size() == 3) {
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);

		return;
	}

	vector<Vertex> tempVertices = vertices;

	while (true) {
		for (auto i = 0; i < int(tempVertices.size()); i++) {
			Vertex previous;
			if (i == 0) {
				previous = tempVertices[tempVertices.size() - 1];
			} else {
				previous = tempVertices[i - 1];
			}

			Vertex current = tempVertices[i];

			Vertex next;
			if (i == tempVertices.size() - 1) {
				next = tempVertices[0];
			} else {
				next = tempVertices[i + 1];
			}

			if (tempVertices.size() == 3) {
				for (auto j = 0; j < int(tempVertices.size()); j++) {
					if (vertices[j].Position == current.Position) indices.push_back(j);
					if (vertices[j].Position == previous.Position) indices.push_back(j);
					if (vertices[j].Position == next.Position) indices.push_back(j);
				}

				tempVertices.clear();
				break;
			}

			if (tempVertices.size() == 4) {
				for (auto j = 0; j < int(vertices.size()); j++) {
					if (vertices[j].Position == current.Position) indices.push_back(j);
					if (vertices[j].Position == previous.Position) indices.push_back(j);
					if (vertices[j].Position == next.Position) indices.push_back(j);
				}

				glm::vec3 tempVector;
				for (auto j = 0; j < int(tempVertices.size()); j++) {
					if (tempVertices[j].Position != current.Position && tempVertices[j].Position != previous.Position && tempVertices[j].Position != next.Position) {
						tempVector = tempVertices[j].Position;
						break;
					}
				}

				for (auto j = 0; j < int(vertices.size()); j++) {
					if (vertices[j].Position == current.Position) indices.push_back(j);
					if (vertices[j].Position == previous.Position) indices.push_back(j);
					if (vertices[j].Position == tempVector) indices.push_back(j);
				}

				tempVertices.clear();
				break;
			}

			float angle = glm::angle(previous.Position - current.Position, next.Position - current.Position) * (180.F / M_PI);
			if (angle <= 0 && angle >= 180) continue;

			auto inTriangle = false;
			for (auto j = 0; j < int(vertices.size()); j++) {
				if (objhelpers::inTriangle(vertices[j].Position, previous.Position, current.Position, next.Position)
					&& vertices[j].Position != previous.Position
					&& vertices[j].Position != current.Position
					&& vertices[j].Position != next.Position) {
					inTriangle = true;
					break;
				}
			}

			if (inTriangle) continue;

			for (auto j = 0; j < int(vertices.size()); j++) {
				if (vertices[j].Position == current.Position) indices.push_back(j);
				if (vertices[j].Position == previous.Position) indices.push_back(j);
				if (vertices[j].Position == next.Position) indices.push_back(j);
			}

			for (auto j = 0; j < int(tempVertices.size()); j++) {
				if (tempVertices[j].Position == current.Position) {
					tempVertices.erase(tempVertices.begin() + j);
					break;
				}
			}

			i = -1;
		}

		if (indices.size() == 0) break;
		if (tempVertices.size() == 0) break;
	}
}