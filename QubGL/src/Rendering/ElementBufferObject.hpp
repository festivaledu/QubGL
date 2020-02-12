#pragma once

#include "GLEW/glew.h"

class ElementBufferObject {
public:
	ElementBufferObject();
	~ElementBufferObject();

	void Bind() const;
	void BufferData(GLenum bufferType, GLsizeiptr bufferSize, const void* bufferData, GLenum drawMode);
	unsigned int GetIndicesCount() const;
	void Unbind() const;

private:
	unsigned int m_ebo = 0;
	unsigned int m_indicesCount = 0;
};