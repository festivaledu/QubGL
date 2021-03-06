//
//  Cube.cpp
//  QubGL
//
//  Copyright © 2020 Team FESTIVAL. All rights reserved.
//

#define _USE_MATH_DEFINES

#include "Cube.hpp"

#include "GLFW/glfw3.h"

#include <cmath>
#include <iostream>

#include "../Rendering/Color.hpp"
#include "../Rendering/Mesh.hpp"
#include "../Rendering/ShaderProgram.hpp"

#include "../Libraries/easing.h"

auto a = 0.F;
auto iteration = 0;

Cube::Cube(const ShaderProgram* shader)
    :m_shader(shader) {

}

void Cube::Draw() {
	if ((m_rotateSide != Side::None) && a != m_rotateAngle) {
        // Approach target angle
		a = easeOutCubic(iteration, m_animationStartAngle, m_rotateAngle, 30);
		iteration++;
	
		for (auto p : GetSide(m_rotateSide)) {
			auto& tf = p->GetTransform();
			auto r = tf.GetRotation();

			glm::vec3 m(p->Position);

			switch (m_rotateSide) {
			case Side::Left:
			case Side::Right:
                // Rotate around x axis
				tf.SetRotation(a, r.y, r.z);
				m = glm::vec3(p->Position) * RotateX(glm::radians(a));
				break;
			case Side::Bottom:
			case Side::Top:
                // Rotate around y axis
				tf.SetRotation(r.x, a, r.z);
				m = glm::vec3(p->Position) * RotateY(glm::radians(a));
				break;
			case Side::Back:
			case Side::Front:
                // Rotate around z axis
				tf.SetRotation(r.x, r.y, a);
				m = glm::vec3(p->Position) * RotateZ(glm::radians(a));
				break;
			default:
				a = 0.F;
			}

            // Adjust model position on rotation curve
			tf.SetTranslation(m.x, m.y, m.z);
		}

		FinishRotate();
	} else {
        // Reset all cube positions after rotating
        for (auto p : m_pointers) {
            auto& tf = p->GetTransform();
            tf.SetTranslation(p->Position.x, p->Position.y, p->Position.z);
        }
    }

    for (auto m : m_models) {
        m.Draw(*m_shader);
    }
}

void Cube::FinishRotate() {
    // Rotation isn't completed
    if (a != m_rotateAngle) return;

    if (m_rotateSide != Side::None) {
        auto pointers = GetSide(m_rotateSide);

        for (auto p : pointers) {
            float c = 0;

            // Update the colors on each model to represent the current cube state
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

                c = p->Colors.y;
                p->Colors.y = p->Colors.z;
                p->Colors.z = c;

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

                c = p->Colors.x;
                p->Colors.x = p->Colors.z;
                p->Colors.z = c;

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

                c = p->Colors.x;
                p->Colors.x = p->Colors.y;
                p->Colors.y = c;

                break;
            }

            // Reset all rotations
            auto& tf = p->GetTransform();
            tf.SetRotation(0.F, 0.F, 0.F);
        }
    }

    // End rotation
    m_isRotating = false;
    m_rotateSide = Side::None;

	iteration = 0;
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

                transform.SetScale(.5F, .5F, .5F);
                transform.SetTranslation(c, l, r);

                m_models.push_back(model);
                m_pointers.push_back(&m_models.back());
            }
        }
    }

    GenerateSides();
}

void Cube::GenerateSides() {
    // Initialize default colors
    // The vectors x, y and z values correspond to the color index of the color shown on each side facing said axis
    m_pointers[0]->Colors = glm::vec3(Color::White, Color::Orange, Color::Blue);
    m_pointers[1]->Colors = glm::vec3(Color::White, Color::Orange, -1);
    m_pointers[2]->Colors = glm::vec3(Color::White, Color::Orange, Color::Green);

    m_pointers[3]->Colors = glm::vec3(-1, Color::Orange, Color::Blue);
    m_pointers[4]->Colors = glm::vec3(-1, Color::Orange, -1);
    m_pointers[5]->Colors = glm::vec3(-1, Color::Orange, Color::Green);

    m_pointers[6]->Colors = glm::vec3(Color::Yellow, Color::Orange, Color::Blue);
    m_pointers[7]->Colors = glm::vec3(Color::Yellow, Color::Orange, -1);
    m_pointers[8]->Colors = glm::vec3(Color::Yellow, Color::Orange, Color::Green);

    m_pointers[9]->Colors = glm::vec3(Color::White, -1, Color::Blue);
    m_pointers[10]->Colors = glm::vec3(Color::White, -1, -1);
    m_pointers[11]->Colors = glm::vec3(Color::White, -1, Color::Green);

    m_pointers[12]->Colors = glm::vec3(-1, -1, Color::Blue);
    m_pointers[13]->Colors = glm::vec3(-1, -1, Color::Green);

    m_pointers[14]->Colors = glm::vec3(Color::Yellow, -1, Color::Blue);
    m_pointers[15]->Colors = glm::vec3(Color::Yellow, -1, -1);
    m_pointers[16]->Colors = glm::vec3(Color::Yellow, -1, Color::Green);

    m_pointers[17]->Colors = glm::vec3(Color::White, Color::Red, Color::Blue);
    m_pointers[18]->Colors = glm::vec3(Color::White, Color::Red, -1);
    m_pointers[19]->Colors = glm::vec3(Color::White, Color::Red, Color::Green);

    m_pointers[20]->Colors = glm::vec3(-1, Color::Red, Color::Blue);
    m_pointers[21]->Colors = glm::vec3(-1, Color::Red, -1);
    m_pointers[22]->Colors = glm::vec3(-1, Color::Red, Color::Green);

    m_pointers[23]->Colors = glm::vec3(Color::Yellow, Color::Red, Color::Blue);
    m_pointers[24]->Colors = glm::vec3(Color::Yellow, Color::Red, -1);
    m_pointers[25]->Colors = glm::vec3(Color::Yellow, Color::Red, Color::Green);
}

bool Cube::GetIsRotating() const {
    return m_isRotating;
}

std::vector<Model*> Cube::GetSide(Side side) {
    std::vector<Model*> s;

    // Returns a side of a cube containing 9 individual cubes
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

    // Blueprint the rotation
	m_animationStartAngle = a;
    m_rotateAngle = a + (int)direction;

    m_rotateDirection = direction;
    m_rotateSide = side;
    m_isRotating = true;
}

glm::mat3 Cube::RotateX(const float angle) {
    // Returns the transformation matrix for an X rotation
    return glm::mat3(1, 0, 0, 0, cos(angle), -sin(angle), 0, sin(angle), cos(angle));
}

glm::mat3 Cube::RotateY(const float angle) {
    // Returns the transformation matrix for an Y rotation
    return glm::mat3(cos(angle), 0, sin(angle), 0, 1, 0, -sin(angle), 0, cos(angle));
}

glm::mat3 Cube::RotateZ(const float angle) {
    // Returns the transformation matrix for an Z rotation
    return glm::mat3(cos(angle), -sin(angle), 0, sin(angle), cos(angle), 0, 0, 0, 1);
}

void Cube::SetShader(const ShaderProgram* shader) {
    m_shader = shader;
}