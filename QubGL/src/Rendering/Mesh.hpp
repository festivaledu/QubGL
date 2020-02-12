#pragma once

#include <string>

class Mesh {
public:
	virtual void Bind() const = 0;
	virtual void Draw() const = 0;
	virtual void Unbind() const = 0;

	std::string GetName() const {
		return m_name;
	}

	void SetName(std::string name) {
		m_name = name;
	}

protected:
	std::string m_name;
	class VertexArrayObject* m_vao;
};