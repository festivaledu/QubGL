//
//  VertexBufferObject.hpp
//  QubGL
//
//  Copyright © 2020 Team FESTIVAL. All rights reserved.
//

#pragma once

#include "GLEW/glew.h"

class VertexBufferObject {
public:
    VertexBufferObject();
    ~VertexBufferObject();

    void Bind() const;
    void BufferData(GLenum bufferType, GLsizeiptr bufferSize, const void* bufferData, GLenum drawMode);
    void Unbind() const;

private:
    unsigned int m_vbo;
};