#include "Vertex.hpp"

#include <random>

Vertex::Vertex(glm::vec3 position, glm::vec4 color, glm::vec3 normal)
    :Position(position), Color(color), Normal(normal) {
    Color = glm::vec4((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, 1.F);
}