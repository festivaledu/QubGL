#define _USE_MATH_DEFINES

#include "Cube.hpp"

#include "GLFW/glfw3.h"

#include <cmath>
#include <iostream>

#include "../Rendering/Color.hpp"
#include "../Rendering/Mesh.hpp"
#include "../Rendering/ShaderProgram.hpp"

auto a = 0.F;

Cube::Cube(const ShaderProgram* shader)
    :m_shader(shader) {

}

void Cube::Draw() {
    if (m_rotateSide != Side::None && a != m_rotateAngle) {
        if (m_rotateDirection == Direction::Clockwise) {
            a -= 1.F;
        } else {
            a += 1.F;
        }

        if (a > 360.F) a = 0;
        if (a < 0.F) a = 360.F;

        for (auto p : GetSide(m_rotateSide)) {
            auto& tf = p->GetTransform();

            glm::vec3 m(p->Position);

            switch (m_rotateSide) {
            case Side::Left:
            case Side::Right:
                tf.SetRotation(a, 0.F, 0.F);
                m = glm::vec3(p->Position) * RotateX(glm::radians(a));
                break;
            case Side::Bottom:
            case Side::Top:
                tf.SetRotation(0.F, a, 0.F);
                m = glm::vec3(p->Position) * RotateY(glm::radians(a));
                break;
            case Side::Back:
            case Side::Front:
                tf.SetRotation(0.F, 0.F, a);
                m = glm::vec3(p->Position) * RotateZ(glm::radians(a));
                break;
            default:
                a = 0.F;
            }

            tf.SetTranslation(m.x, m.y, m.z);
        }

        FinishRotate();
    }

    for (auto m : m_models) {
        m.Draw(*m_shader);
    }
}

void Cube::FinishRotate() {
    if (a != m_rotateAngle) return;

    for (auto p : GetSide(m_rotateSide)) {
        switch (m_rotateSide) {
        case Side::Left:
        case Side::Right:
            if (m_rotateDirection == Direction::Clockwise) {
                auto y = p->Position.y;
                p->Position.y = p->Position.z;
                p->Position.z = (y * -1) + 0.F;
            } else {
                auto z = p->Position.z;
                p->Position.z = p->Position.y;
                p->Position.y = (z * -1) + 0.F;
            }
            break;
        case Side::Bottom:
        case Side::Top:
            if (m_rotateDirection == Direction::Clockwise) {
                auto z = p->Position.z;
                p->Position.z = p->Position.x;
                p->Position.x = (z * -1) + 0.F;
            } else {
                auto x = p->Position.x;
                p->Position.x = p->Position.z;
                p->Position.z = (x * -1) + 0.F;
            }
            break;
        case Side::Back:
        case Side::Front:
            if (m_rotateDirection == Direction::Clockwise) {
                auto x = p->Position.x;
                p->Position.x = p->Position.y;
                p->Position.y = (x * -1) + 0.F;
            } else {
                auto y = p->Position.y;
                p->Position.y = p->Position.x;
                p->Position.x = (y * -1) + 0.F;
            }
            break;
        }
    }

    m_isRotating = false;
    m_rotateSide = Side::None;
}

void Cube::GenerateModels(const Mesh mesh) {
    // Layers
    for (auto l = -1; l <= 1; l++) {
        // Columns
        for (auto c = -1; c <= 1; c++) {
            // Rows
            for (auto r = -1; r <= 1; r++) {
                // Skip innermost cube
                if (l == 0 && c == 0 && r == 0) continue;

                Model model(mesh);
                model.Position = glm::vec3(c, l, r);

                auto& transform = model.GetTransform();

                transform.SetScale(.4F, .4F, .4F);
                transform.SetTranslation(c, l, r);

                /*if (l == 1) {
                    a = 45.F;

                    transform.SetRotation(0.F, a, 0.F);

                    auto translation = glm::vec3(c, l, r) * RotateY(glm::radians(a));
                    transform.SetTranslation(translation.x, translation.y, translation.z);
                }*/

                m_models.push_back(model);
                m_pointers.push_back(&m_models.back());
            }
        }
    }

    GenerateSides();
}

void Cube::GenerateSides() {
    m_pointers[0]->Colors = { Color::Blue, Color::Orange, Color::White };
    m_pointers[1]->Colors = { Color::Orange, Color::White };
    m_pointers[2]->Colors = { Color::Green, Color::Orange, Color::White };

    m_pointers[3]->Colors = { Color::Blue, Color::Orange };
    m_pointers[4]->Colors = { Color::Orange };
    m_pointers[5]->Colors = { Color::Green, Color::Orange };

    m_pointers[6]->Colors = { Color::Blue, Color::Orange, Color::Yellow };
    m_pointers[7]->Colors = { Color::Orange, Color::Yellow };
    m_pointers[8]->Colors = { Color::Green, Color::Orange, Color::Yellow };

    m_pointers[9]->Colors = { Color::Blue, Color::White };
    m_pointers[10]->Colors = { Color::White };
    m_pointers[11]->Colors = { Color::Green, Color::White };

    m_pointers[12]->Colors = { Color::Blue };
    m_pointers[13]->Colors = { Color::Green };

    m_pointers[14]->Colors = { Color::Blue, Color::Yellow };
    m_pointers[15]->Colors = { Color::Yellow };
    m_pointers[16]->Colors = { Color::Green, Color::Yellow };

    m_pointers[17]->Colors = { Color::Red, Color::Blue, Color::White };
    m_pointers[18]->Colors = { Color::Red, Color::White };
    m_pointers[19]->Colors = { Color::Green, Color::Green, Color::White };

    m_pointers[20]->Colors = { Color::Red, Color::Blue };
    m_pointers[21]->Colors = { Color::Red };
    m_pointers[22]->Colors = { Color::Green, Color::Red };

    m_pointers[23]->Colors = { Color::Red, Color::Blue, Color::Yellow };
    m_pointers[24]->Colors = { Color::Red, Color::Yellow };
    m_pointers[25]->Colors = { Color::Green, Color::Red, Color::Yellow };
}

bool Cube::GetIsRotating() const {
    return m_isRotating;
}

std::vector<Model*> Cube::GetSide(Side side) {
    std::vector<Model*> s;

    for (auto p : m_pointers) {
        switch (side) {
        case Side::Bottom:
            if (p->Position.y == -1) s.push_back(p);
            break;
        case Side::Left:
            if (p->Position.x == -1) s.push_back(p);
            break;
        case Side::Back:
            if (p->Position.z == -1) s.push_back(p);
            break;
        case Side::Front:
            if (p->Position.z == 1) s.push_back(p);
            break;
        case Side::Right:
            if (p->Position.x == 1) s.push_back(p);
            break;
        case Side::Top:
            if (p->Position.y == 1) s.push_back(p);
            break;
        default:
            return s;
        }

        if (s.size() == 9) break;
    }

    return s;
}

void Cube::Rotate(Side side, Direction direction) {
    if (side == Side::None) return;

    auto& rotation = GetSide(side)[0]->GetTransform().GetRotation();

    switch (side) {
    case Side::Left:
    case Side::Right:
        a = rotation.x;
        break;
    case Side::Bottom:
    case Side::Top:
        a = rotation.y;
        break;
    case Side::Back:
    case Side::Front:
        a = rotation.z;
        break;
    default:
        a = 0.F;
    }

    m_rotateAngle = a + (int)direction;
    if (m_rotateAngle < 0) m_rotateAngle += 360.F;
    if (m_rotateAngle >= 360.F) m_rotateAngle -= 360.F;

    m_rotateDirection = direction;
    m_rotateSide = side;
    m_isRotating = true;
}

glm::mat3 Cube::RotateX(const float angle) {
    return glm::mat3(1, 0, 0, 0, cos(angle), -sin(angle), 0, sin(angle), cos(angle));
}

glm::mat3 Cube::RotateY(const float angle) {
    return glm::mat3(cos(angle), 0, sin(angle), 0, 1, 0, -sin(angle), 0, cos(angle));
}

glm::mat3 Cube::RotateZ(const float angle) {
    return glm::mat3(cos(angle), -sin(angle), 0, sin(angle), cos(angle), 0, 0, 0, 1);
}

void Cube::SetShader(const ShaderProgram* shader) {
    m_shader = shader;
}