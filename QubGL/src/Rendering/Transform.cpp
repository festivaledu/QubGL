#include "Transform.hpp"

Transform::Transform()
    :m_rotation(0.F), m_scale(1.F), m_translation(0.F) { }

glm::vec3 Transform::GetRotation() const {
    return m_rotation;
}

glm::vec3 Transform::GetScale() const {
    return m_scale;
}

glm::vec3 Transform::GetTranslation() const {
    return m_translation;
}

void Transform::SetRotation(float pitch, float yaw, float roll) {
    m_rotation.x = pitch;
    m_rotation.y = yaw;
    m_rotation.z = roll;
}

void Transform::SetScale(float x, float y, float z) {
    m_scale.x = x;
    m_scale.y = y;
    m_scale.z = z;
}

void Transform::SetTranslation(float x, float y, float z) {
    m_translation.x = x;
    m_translation.y = y;
    m_translation.z = z;
}