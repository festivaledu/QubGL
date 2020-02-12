#pragma once

enum VertexAttribLocation {
	Position,
	VertexColor,
	Normal,
};

class VertexArrayObject {
public:
	VertexArrayObject();
	~VertexArrayObject();

	void Bind() const;
	void SetupAttribPointers();
	void Unbind() const;

private:
	unsigned int m_vao;
};