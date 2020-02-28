//
//  VertexArrayObject.hpp
//  QubGL
//
//  Copyright © 2020 Team FESTIVAL. All rights reserved.
//

#pragma once

enum VertexAttribLocation {
    Position = 0,
    VertexColor = 1,
    Normal = 2,
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