//
//  VertexArrayObject.cpp
//  QubGL
//
//  Copyright © 2020 Team FESTIVAL. All rights reserved.
//

#include "VertexArrayObject.hpp"

#include "GLEW/glew.h"

#include "Vertex.hpp"

VertexArrayObject::VertexArrayObject() {
    glGenVertexArrays(1, &m_vao);
}

VertexArrayObject::~VertexArrayObject() {
    glDeleteVertexArrays(1, &m_vao);
}

void VertexArrayObject::Bind() const {
    glBindVertexArray(m_vao);
}

void VertexArrayObject::SetupAttribPointers() {
    glEnableVertexAttribArray(VertexAttribLocation::Position);
    glEnableVertexAttribArray(VertexAttribLocation::VertexColor);
    glEnableVertexAttribArray(VertexAttribLocation::Normal);

    glVertexAttribPointer(VertexAttribLocation::Position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));
    glVertexAttribPointer(VertexAttribLocation::VertexColor, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));
    glVertexAttribPointer(VertexAttribLocation::Normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Normal));
}

void VertexArrayObject::Unbind() const {
    glBindVertexArray(0);
}