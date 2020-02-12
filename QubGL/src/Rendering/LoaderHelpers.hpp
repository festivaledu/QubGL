#pragma once

#include "GLM/gtx/normal.hpp"

#include <string>
#include <vector>

namespace objhelpers {
	enum VertexType {
		Position = 1,
		PositionAndTexture = 2,
		PositionAndNormal = 3,
		PositionAndTextureAndNormal = 4,
	};

	template <class T>
	const T& getElement(const std::vector<T>& elements, std::string& index) {
		auto i = std::stoi(index);

		if (i < 0) {
			i += int(elements.size());
		} else {
			i--;
		}

		return elements[i];
	}

	std::string firstToken(const std::string& in) {
		if (!in.empty()) {
			auto tokenStart = in.find_first_not_of(" \t");
			auto tokenEnd = in.find_first_of(" \t", tokenStart);

			if (tokenStart != std::string::npos && tokenEnd != std::string::npos) {
				return in.substr(tokenStart, tokenEnd - tokenStart);
			} else if (tokenStart != std::string::npos) {
				return in.substr(tokenStart);
			}
		}

		return "";
	}

	bool inTriangle(glm::vec3 point, glm::vec3 triangleX, glm::vec3 triangleY, glm::vec3 triangleZ) {
		auto isWithinPrism = isOnSameSide(point, triangleX, triangleY, triangleZ) && isOnSameSide(point, triangleY, triangleX, triangleZ) && isOnSameSide(point, triangleZ, triangleX, triangleY);

		if (!isWithinPrism) return false;

		auto n = glm::triangleNormal(triangleX, triangleY, triangleZ);
		auto projection = projectVector(point, n);

		return glm::length2(projection) == 0;
	}

	bool isOnSameSide(glm::vec3 point1, glm::vec3 point2, glm::vec3 a, glm::vec3 b) {
		auto cp1 = glm::cross(b - a, point1 - a);
		auto cp2 = glm::cross(b - a, point2 - a);

		return glm::dot(cp1, cp2) >= 0;
	}

	glm::vec3 projectVector(const glm::vec3 a, const glm::vec3 b) {
		auto bn = b / glm::length2(b);
		return bn * glm::dot(a, bn);
	}

	void split(const std::string& in, std::vector<std::string>& out, std::string token) {
		out.clear();

		std::string t;

		for (auto i = 0; i < int(in.size()); i++) {
			auto test = in.substr(i, token.size());

			if (test == token) {
				if (!t.empty()) {
					out.push_back(t);
					t.clear();
					i += (int)token.size() - 1;
				} else {
					out.push_back("");
				}
			} else if (i + token.size() >= in.size()) {
				t += in.substr(i, token.size());
				out.push_back(t);
				break;
			} else {
				t += in[i];
			}
		}
	}

	std::string tail(const std::string& in) {
		auto tokenStart = in.find_first_not_of(" \t");
		auto spaceStart = in.find_first_of(" \t", tokenStart);
		auto tailStart = in.find_first_not_of(" \t", spaceStart);
		auto tailEnd = in.find_last_not_of(" \t");

		if (tailStart != std::string::npos && tailEnd != std::string::npos) {
			return in.substr(tailStart, tailEnd - tailStart + 1);
		} else if (tailStart != std::string::npos) {
			return in.substr(tailStart);
		}

		return "";
	}
}