#pragma once

#include "GLM/gtc/matrix_transform.hpp"

#include <string>

class Model;

class ShaderProgram {
public:
    ShaderProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

    void Bind() const;
    void SetColorOverrides(Model& model) const;
    void SetDirectionalLight(struct DirectionalLight dirLight);
    void SetModelMatrix(Model& model) const;
    void SetProjectionMatrix(const glm::mat4& projectionMatrix);
    void SetViewMatrix(const glm::mat4& viewMatrix);
    void Unbind() const;

private:
    std::string ParseFile(const std::string& shaderFilePath);

    unsigned int m_programId;
};