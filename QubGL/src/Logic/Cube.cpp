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
	if ((m_rotateSide != Side::None || m_rotateAxis != Axis::None) && a != m_rotateAngle) {
		a += (m_rotateDirection == Direction::Clockwise) ? -1.F : 1.F;

        if (a > 360.F) a = 0.F;
        if (a < 0.F) a = 360.F;
	
		if (m_rotateSide != Side::None) {
			for (auto p : GetSide(m_rotateSide)) {
				auto& tf = p->GetTransform();
				auto r = tf.GetRotation();

				glm::vec3 m(p->Position);

				switch (m_rotateSide) {
				case Side::Left:
				case Side::Right:
					tf.SetRotation(a, r.y, r.z);
					m = glm::vec3(p->Position) * RotateX(glm::radians(a));
					break;
				case Side::Bottom:
				case Side::Top:
					tf.SetRotation(r.x, a, r.z);
					m = glm::vec3(p->Position) * RotateY(glm::radians(a));
					break;
				case Side::Back:
				case Side::Front:
					tf.SetRotation(r.x, r.y, a);
					m = glm::vec3(p->Position) * RotateZ(glm::radians(a));
					break;
				default:
					a = 0.F;
				}

				tf.SetTranslation(m.x, m.y, m.z);
			}

			FinishRotate();
		} else if (m_rotateAxis != Axis::None) {
			for (auto p : m_pointers) {
				auto& tf = p->GetTransform();
				auto r = tf.GetRotation();

				glm::vec3 m(p->Position);
				
				switch (m_rotateAxis) {
				case Axis::Y:
					tf.SetRotation(a, r.y, r.z);
					m = glm::vec3(p->Position) * RotateX(glm::radians(a));
					break;
				case Axis::Z:
					tf.SetRotation(r.x, a, r.z);
					m = glm::vec3(p->Position) * RotateY(glm::radians(a));
					break;
				case Axis::X:
					tf.SetRotation(r.x, r.y, a);
					m = glm::vec3(p->Position) * RotateZ(glm::radians(a));
					break;
				default:
					a = 0.F;
				}

				tf.SetTranslation(m.x, m.y, m.z);
			}
			
			FinishRotate();
		}
	} else {
        for (auto p : m_pointers) {
            auto& tf = p->GetTransform();
            tf.SetTranslation(p->Position.x, p->Position.y, p->Position.z);
        }
    }

    for (auto m : m_models) {
        m.Draw(*m_shader);
    }
}

std::vector<std::string> sides = { "Keine", "Unten", "Links", "Hinten", "Vorne", "Rechts", "Oben" };
std::vector<int> colors = { 42, 41, 44, 43, 47, 103 };

void Cube::FinishRotate() {
    if (a != m_rotateAngle) return;

    auto pointers = GetSide(m_rotateSide);

    std::cout << std::endl << sides[(int)m_rotateSide] << " gedreht " << (m_rotateDirection == Direction::Clockwise ? "im" : "gegen den") << " Uhrzeigersinn" << std::endl;

    for (auto p : pointers) {
        for (auto c : p->Colors) {
            // std::cout << colors[c] << " ";
            std::cout << "\x1B[" + std::to_string(colors[c]) + "m   \033[0m";
        }

        std::cout << std::endl << "(X: " << p->Position.x << " | Y: " << p->Position.y << " | Z: " << p->Position.z << " )" << std::endl;

        /*if ((m_rotateDirection == Direction::Clockwise && m_rotateSide == Side::Left) || (m_rotateDirection == Direction::CounterClockwise && m_rotateSide == Side::Right)) {
            auto z = p->Position.z;
            p->Position.z = p->Position.y;
            p->Position.y = z * -1;
        } else if ((m_rotateDirection == Direction::CounterClockwise && m_rotateSide == Side::Left) || (m_rotateDirection == Direction::Clockwise && m_rotateSide == Side::Right)) {
            auto y = p->Position.y;
            p->Position.y = p->Position.z;
            p->Position.z = y * -1;
        } else if ((m_rotateDirection == Direction::Clockwise && m_rotateSide == Side::Bottom) || (m_rotateDirection == Direction::CounterClockwise && m_rotateSide == Side::Top)) {
            auto x = p->Position.x;
            p->Position.x = p->Position.z;
            p->Position.z = x * -1;
        } else if ((m_rotateDirection == Direction::CounterClockwise && m_rotateSide == Side::Bottom) || (m_rotateDirection == Direction::Clockwise && m_rotateSide == Side::Top)) {
            auto z = p->Position.z;
            p->Position.z = p->Position.x;
            p->Position.x = z * -1;
        } else if ((m_rotateDirection == Direction::Clockwise && m_rotateSide == Side::Back) || (m_rotateDirection == Direction::CounterClockwise && m_rotateSide == Side::Front)) {
            auto y = p->Position.y;
            p->Position.y = p->Position.x;
            p->Position.x = y * -1;
        } else if ((m_rotateDirection == Direction::CounterClockwise && m_rotateSide == Side::Back) || (m_rotateDirection == Direction::Clockwise && m_rotateSide == Side::Front)) {
            auto x = p->Position.x;
            p->Position.x = p->Position.y;
            p->Position.y = x * -1;
        }*/

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

        std::cout << "(X: " << p->Position.x << " | Y: " << p->Position.y << " | Z: " << p->Position.z << " )" << std::endl;
    }

    m_isRotating = false;
	m_rotateAxis = Axis::None;
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

                if (l == 1 && c == -1 && r == 1) {
                    transform.SetRotation(0.F, 0.F, 0.F);
                }

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
    m_pointers[19]->Colors = { Color::Red, Color::Green, Color::White };

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

    auto rotation = GetSide(side)[0]->GetTransform().GetRotation();

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

void Cube::Rotate(Axis axis, Direction direction) {
	if (axis == Axis::None) return;
	
	auto rotation = m_pointers.front()->GetTransform().GetRotation();
	
	switch (axis) {
    case Axis::Y:
        a = rotation.x;
        break;
    case Axis::Z:
        a = rotation.y;
        break;
    case Axis::X:
        a = rotation.z;
        break;
    default:
        a = 0.F;
    }
	
	m_rotateAngle = a + (int)direction;
    if (m_rotateAngle < 0) m_rotateAngle += 360.F;
    if (m_rotateAngle >= 360.F) m_rotateAngle -= 360.F;

    m_rotateDirection = direction;
    m_rotateAxis = axis;
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