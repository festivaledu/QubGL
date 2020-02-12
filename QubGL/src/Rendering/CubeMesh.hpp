#pragma once

#include <vector>

#include "Mesh.hpp"

class Vertex;

class CubeMesh : public Mesh {
public:
	CubeMesh(): m_ebo(nullptr), m_vbo(nullptr) { }
	CubeMesh(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices);

	virtual void Bind() const override;
	virtual void Draw() const override;
	virtual void Unbind() const override;

private:
	void Load(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices);
	void Unload();

	class ElementBufferObject* m_ebo;
	bool m_isLoaded = false;
	class VertexBufferObject* m_vbo;
};