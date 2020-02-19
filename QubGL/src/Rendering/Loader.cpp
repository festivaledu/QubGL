#define _USE_MATH_DEFINES

#include "Loader.hpp"

#include "GLM/gtx/matrix_cross_product.hpp"
#include "GLM/gtx/vector_angle.hpp"

#include <cmath>
#include <fstream>

#include "LoaderHelpers.hpp"
#include "Mesh.hpp"
#include "Vertex.hpp"

using namespace std;

Loader::~Loader() {
    Meshes.clear();
}

void Loader::GenerateVerticesFromRawObj(vector<Vertex>& vertices, const vector<glm::vec3>& positions, const vector<glm::vec2>& coordinates, const vector<glm::vec3>& normals, string currentLine) {
    vector<string> face, sVertices;
    Vertex vertex;

    objhelpers::split(objhelpers::tail(currentLine), face, " ");

    auto noNormal = false;

    for (auto i = 0; i < int(face.size()); i++) {
        objhelpers::VertexType vertexType = objhelpers::VertexType::None;

        objhelpers::split(face[i], sVertices, "/");

        if (sVertices.size() == 1) {
            vertexType = objhelpers::VertexType::Position;
        } else if (sVertices.size() == 2) {
            vertexType = objhelpers::VertexType::PositionAndTexture;
        } else if (sVertices.size() == 3) {
            if (sVertices[1] == "") {
                vertexType = objhelpers::VertexType::PositionAndNormal;
            } else {
                vertexType = objhelpers::VertexType::PositionAndTextureAndNormal;
            }
        }

        switch (vertexType) {
        case objhelpers::VertexType::Position: {
            vertex.Position = objhelpers::getElement(positions, sVertices[0]);
            vertex.TextureCoordinates = glm::vec2(0.F);
            noNormal = true;
            vertices.push_back(vertex);
            break;
        }
        case objhelpers::VertexType::PositionAndTexture: {
            vertex.Position = objhelpers::getElement(positions, sVertices[0]);
            vertex.TextureCoordinates = objhelpers::getElement(coordinates, sVertices[1]);
            noNormal = true;
            vertices.push_back(vertex);
            break;
        }
        case objhelpers::VertexType::PositionAndNormal: {
            vertex.Position = objhelpers::getElement(positions, sVertices[0]);
            vertex.TextureCoordinates = glm::vec2(0.F);
            vertex.Normal = objhelpers::getElement(normals, sVertices[2]);
            vertices.push_back(vertex);
            break;
        }
        case objhelpers::VertexType::PositionAndTextureAndNormal: {
            vertex.Position = objhelpers::getElement(positions, sVertices[0]);
            vertex.TextureCoordinates = objhelpers::getElement(coordinates, sVertices[1]);
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

bool Loader::LoadMaterials(const string& mtlFilePath) {
    if (mtlFilePath.substr(mtlFilePath.size() - 4, mtlFilePath.size()) != ".mtl") return false;

    ifstream stream(mtlFilePath);

    if (!stream.is_open()) return false;

    auto isListening = false;

    Material tempMaterial;

    string currentLine;
    while (getline(stream, currentLine)) {
        if (objhelpers::firstToken(currentLine) == "newmtl") {
            if (!isListening) {
                isListening = true;

                if (currentLine.size() > 7) {
                    tempMaterial.Name = objhelpers::tail(currentLine);
                } else {
                    tempMaterial.Name = "none";
                }
            } else {
                Materials.push_back(tempMaterial);
                tempMaterial = Material();

                if (currentLine.size() > 7) {
                    tempMaterial.Name = objhelpers::tail(currentLine);
                } else {
                    tempMaterial.Name = "none";
                }
            }
        }

        if (objhelpers::firstToken(currentLine) == "Ka") {
            vector<string> temp;
            objhelpers::split(objhelpers::tail(currentLine), temp, " ");

            if (temp.size() != 3) continue;

            tempMaterial.Ka.x = stof(temp[0]);
            tempMaterial.Ka.y = stof(temp[1]);
            tempMaterial.Ka.z = stof(temp[2]);
        }

        if (objhelpers::firstToken(currentLine) == "Kd") {
            vector<string> temp;
            objhelpers::split(objhelpers::tail(currentLine), temp, " ");

            if (temp.size() != 3) continue;

            tempMaterial.Kd.x = stof(temp[0]);
            tempMaterial.Kd.y = stof(temp[1]);
            tempMaterial.Kd.z = stof(temp[2]);
        }

        if (objhelpers::firstToken(currentLine) == "Ks") {
            vector<string> temp;
            objhelpers::split(objhelpers::tail(currentLine), temp, " ");

            if (temp.size() != 3) continue;

            tempMaterial.Ks.x = stof(temp[0]);
            tempMaterial.Ks.y = stof(temp[1]);
            tempMaterial.Ks.z = stof(temp[2]);
        }

        if (objhelpers::firstToken(currentLine) == "Ns") {
            tempMaterial.Ns = stof(objhelpers::tail(currentLine));
        }

        if (objhelpers::firstToken(currentLine) == "Ni") {
            tempMaterial.Ni = stof(objhelpers::tail(currentLine));
        }

        if (objhelpers::firstToken(currentLine) == "d") {
            tempMaterial.D = stof(objhelpers::tail(currentLine));
        }

        if (objhelpers::firstToken(currentLine) == "illum") {
            tempMaterial.Illum = stoi(objhelpers::tail(currentLine));
        }

        if (objhelpers::firstToken(currentLine) == "map_Ka") {
            tempMaterial.MapKa = objhelpers::tail(currentLine);
        }

        if (objhelpers::firstToken(currentLine) == "map_Kd") {
            tempMaterial.MapKd = objhelpers::tail(currentLine);
        }

        if (objhelpers::firstToken(currentLine) == "map_Ks") {
            tempMaterial.MapKs = objhelpers::tail(currentLine);
        }

        if (objhelpers::firstToken(currentLine) == "map_Ns") {
            tempMaterial.MapKa = objhelpers::tail(currentLine);
        }

        if (objhelpers::firstToken(currentLine) == "map_d") {
            tempMaterial.MapD = objhelpers::tail(currentLine);
        }

        if (objhelpers::firstToken(currentLine) == "map_Bump" || objhelpers::firstToken(currentLine) == "map_bump" || objhelpers::firstToken(currentLine) == "bump") {
            tempMaterial.MapBump = objhelpers::tail(currentLine);
        }
    }

    Materials.push_back(tempMaterial);

    return !Materials.empty();
}

bool Loader::LoadObjects(const string& objFilePath) {
    if (objFilePath.substr(objFilePath.size() - 4, 4) != ".obj") return false;

    ifstream stream(objFilePath);

    if (!stream.is_open()) return false;

    Indices.clear();
    Meshes.clear();
    Vertices.clear();

    vector<glm::vec3> positions;
    vector<glm::vec2> coordinates;
    vector<glm::vec3> normals;

    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<string> materialNames;

    auto isListening = false;

    string meshName;
    Mesh tempMesh;

    string currentLine;
    while (getline(stream, currentLine)) {
        if (objhelpers::firstToken(currentLine) == "o" || objhelpers::firstToken(currentLine) == "g" || currentLine[0] == 'g') {
            if (!isListening) {
                isListening = true;
            } else {
                if (!indices.empty() && !vertices.empty()) {
                    tempMesh = Mesh(vertices, indices);
                    tempMesh.SetName(meshName);

                    Meshes.push_back(tempMesh);

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
            vector<string> sPosition;
            glm::vec3 vPosition;

            objhelpers::split(objhelpers::tail(currentLine), sPosition, " ");

            vPosition.x = stof(sPosition[0]);
            vPosition.y = stof(sPosition[1]);
            vPosition.z = stof(sPosition[2]);

            positions.push_back(vPosition);
        }

        if (objhelpers::firstToken(currentLine) == "vt") {
            vector<string> sTexture;
            glm::vec2 vTexture;

            objhelpers::split(objhelpers::tail(currentLine), sTexture, " ");

            vTexture.x = stof(sTexture[0]);
            vTexture.y = stof(sTexture[1]);

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
                Vertices.push_back(vVertices[i]);
            }

            vector<unsigned int> vIndices;
            TriangulateVertices(vIndices, vVertices);

            for (auto i = 0; i < int(vIndices.size()); i++) {
                auto ii = (unsigned int)((vertices.size() - vVertices.size()) + vIndices[i]);
                indices.push_back(ii);

                ii = (unsigned int)((Vertices.size() - vVertices.size()) + vIndices[i]);
                Indices.push_back(ii);
            }
        }

        if (objhelpers::firstToken(currentLine) == "usemtl") {
            materialNames.push_back(objhelpers::tail(currentLine));

            if (!indices.empty() && !vertices.empty()) {
                tempMesh = Mesh(vertices, indices);
                tempMesh.SetName(meshName);

                while (true) {
                    tempMesh.SetName(meshName + "_2");

                    for (auto& m : Meshes) {
                        if (m.GetName() == tempMesh.GetName()) continue;
                    }

                    break;
                }

                Meshes.push_back(tempMesh);

                vertices.clear();
                indices.clear();
            }
        }

        if (objhelpers::firstToken(currentLine) == "mtllib") {
            vector<string> tempMaterialPath;
            objhelpers::split(objFilePath, tempMaterialPath, "/");

            string materialPath = "";

            if (tempMaterialPath.size() != 1) {
                for (auto i = 0; i < tempMaterialPath.size() - 1; i++) {
                    materialPath += tempMaterialPath[i] + "/";
                }
            }

            materialPath += objhelpers::tail(currentLine);

            LoadMaterials(materialPath);
        }
    }

    if (!indices.empty() && !vertices.empty()) {
        tempMesh = Mesh(vertices, indices);
        tempMesh.SetName(meshName);

        Meshes.push_back(tempMesh);
    }

    stream.close();

    for (auto i = 0; i < materialNames.size(); i++) {
        auto materialName = materialNames[i];

        for (auto j = 0; j < Materials.size(); j++) {
            if (Materials[j].Name == materialName) {
                Meshes[i].SetMaterial(Materials[j]);
                break;
            }
        }
    }

    return Indices.empty() && Meshes.empty() && Vertices.empty();
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
                previous = tempVertices[(size_t)i - 1];
            }

            Vertex current = tempVertices[i];

            Vertex next;
            if (i == tempVertices.size() - 1) {
                next = tempVertices[0];
            } else {
                next = tempVertices[(size_t)i + 1];
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

                glm::vec3 tempVector(0.F);
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

            float angle = glm::angle(previous.Position - current.Position, next.Position - current.Position) * (180.F / (float)M_PI);
            if (angle <= 0 || angle >= 180) continue; // was &&

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