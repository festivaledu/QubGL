#pragma once

#include "GLM/vec3.hpp"

class Transform {
public:
    Transform();

    glm::vec3 GetRotation() const;
    glm::vec3 GetScale() const;
    glm::vec3 GetTranslation() const;

    void SetRotation(float pitch, float yaw, float roll);
    void SetScale(float x, float y, float z);
    void SetTranslation(float x, float y, float z);

private:
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
    glm::vec3 m_translation;
};