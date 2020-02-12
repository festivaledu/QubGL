#pragma once

#include "GLM/vec4.hpp"

#include "Light.hpp"

struct PointLight : Light {
public:
	glm::vec4 Position;
};