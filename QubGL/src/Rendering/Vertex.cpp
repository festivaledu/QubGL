#include "Vertex.hpp"

#include <iostream>
#include <map>
#include <random>
#include <string>

static int i = 0;
static int n = 0;

static glm::vec4 c[] = {
    glm::vec4(0.F, .6F, .28F, 1.F), // Green
    glm::vec4(.725F, 0.F, 0.F, 1.F), // Red
    glm::vec4(0.F, .27F, .678F, 1.F), // Blue
    glm::vec4(1.F, .349F, 0.F, 1.F), // Orange
    glm::vec4(1.F), // White
    glm::vec4(1.F, .835F, 0.F, 1.F), // Yellow
};

static std::map<int, int> faceColors = {
    std::pair<int, int>(2, 0),
    std::pair<int, int>(28, 0),
    std::pair<int, int>(3, 1),
    std::pair<int, int>(29, 1),
    std::pair<int, int>(25, 2),
    std::pair<int, int>(43, 2),
    std::pair<int, int>(0, 3),
    std::pair<int, int>(26, 3),
    std::pair<int, int>(1, 4),
    std::pair<int, int>(27, 4),
    std::pair<int, int>(4, 5),
    std::pair<int, int>(30, 5),
};

Vertex::Vertex(glm::vec3 position, glm::vec4 color, glm::vec3 normal)
    :Position(position), Color(color), Normal(normal) {

    if (faceColors.find(i) != faceColors.end()) {
        Color = c[faceColors[i]];
    } else {
        Color = glm::vec4(.3F, .3F, .3F, 1.F);
    }

    i++;

    //Color = glm::vec4((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, 1.F);
}