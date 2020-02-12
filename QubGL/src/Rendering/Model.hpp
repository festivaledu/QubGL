#pragma once

#include "Transform.hpp"

class Model {
public:
	Model(const class Mesh& mesh);
	~Model();

	void Draw(const class ShaderProgram& shader);
	Transform& GetTransform();

private:
	const class Mesh& m_mesh;
	Transform m_transform;
};