//
//  ElementBufferObject.cpp
//  QubGL
//
//  Copyright © 2020 Team FESTIVAL. All rights reserved.
//

#include "ElementBufferObject.hpp"

#include "GLEW/glew.h"

ElementBufferObject::ElementBufferObject() {
    glGenBuffers(1, &m_ebo);
}

ElementBufferObject::~ElementBufferObject() {
    glDeleteBuffers(1, &m_ebo);
}

void ElementBufferObject::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
}

void ElementBufferObject::BufferData(GLenum bufferType, GLsizeiptr bufferSize, const void* bufferData, GLenum drawMode) {
    glBufferData(bufferType, bufferSize, bufferData, drawMode);
    m_indicesCount = (int)(bufferSize / sizeof(unsigned int));
}

unsigned int ElementBufferObject::GetIndicesCount() const {
    return m_indicesCount;
}

void ElementBufferObject::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}