#pragma once

#include <GLM/mat3x3.hpp>

#include <list>
#include <vector>

#include "../Rendering/Model.hpp"

class Mesh;
class ShaderProgram;

enum class Direction {
    Clockwise = -90,
    CounterClockwise = 90,
};

enum class Side {
    None,
    Bottom,
    Left,
    Back,
    Front,
    Right,
    Top,
};

class Cube {
public:
    Cube(const ShaderProgram* shader);
    ~Cube() { }

    void Draw();
    void GenerateModels(const Mesh mesh);
    bool GetIsRotating() const;
    void Rotate(Side side, Direction direction = Direction::Clockwise);
    void SetShader(const ShaderProgram* shader);

private:
    void FinishRotate();
    void GenerateSides();
    std::vector<Model*> GetSide(Side side);
    glm::mat3 RotateX(const float angle);
    glm::mat3 RotateY(const float angle);
    glm::mat3 RotateZ(const float angle);

	double m_animationStartAngle = 0;
    bool m_isRotating = false;
    std::list<Model> m_models;
    std::vector<Model*> m_pointers;
    float m_rotateAngle = 0.F;
    Direction m_rotateDirection = Direction::Clockwise;
    Side m_rotateSide = Side::None;
    const ShaderProgram* m_shader;
};