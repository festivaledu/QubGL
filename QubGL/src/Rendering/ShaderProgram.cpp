#include "ShaderProgram.hpp"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include <fstream> // -> ifstream
#include <sstream>
#include <vector>

#include "Lighting/DirectionalLight.hpp"
#include "Model.hpp"

using namespace std;

ShaderProgram::ShaderProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
    auto vertexSource = ParseFile(vertexShaderFilePath);
    auto fragmentSource = ParseFile(fragmentShaderFilePath);

    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);

    auto source = (const GLchar*)vertexSource.c_str();

    glShaderSource(vertexShader, 1, &source, 0);
    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

        glDeleteShader(vertexShader);

        cout << &infoLog[0] << endl;
        return;
    }

    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    source = (const GLchar*)fragmentSource.c_str();

    glShaderSource(fragmentShader, 1, &source, 0);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);

        cout << &infoLog[0] << endl;
        return;
    }

    m_programId = glCreateProgram();

    glAttachShader(m_programId, vertexShader);
    glAttachShader(m_programId, fragmentShader);

    glLinkProgram(m_programId);

    GLint isLinked = 0;
    glGetProgramiv(m_programId, GL_LINK_STATUS, (int*)&isLinked);

    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &maxLength);

        vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(m_programId, maxLength, &maxLength, &infoLog[0]);

        glDeleteProgram(m_programId);
        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);

        cout << &infoLog[0] << endl;
        return;
    }

    glDetachShader(m_programId, vertexShader);
    glDetachShader(m_programId, fragmentShader);
}

void ShaderProgram::Bind() const {
    glUseProgram(m_programId);
}

string ShaderProgram::ParseFile(const string& shaderFilePath) {
    ifstream stream(shaderFilePath);

    string line;
    stringstream ss;

    while (getline(stream, line)) {
        ss << line << "\n";
    }

    return ss.str();
}

void ShaderProgram::SetDirectionalLight(DirectionalLight dirLight) {
    auto ambientId = glGetUniformLocation(m_programId, "dirLight.ambient");
    glUniform4fv(ambientId, 1, &dirLight.Ambient[0]);

    auto diffuseId = glGetUniformLocation(m_programId, "dirLight.diffuse");
    glUniform4fv(diffuseId, 1, &dirLight.Diffuse[0]);

    auto directionId = glGetUniformLocation(m_programId, "dirLight.direction");
    glUniform4fv(directionId, 1, &dirLight.Direction[0]);
}

void ShaderProgram::SetModelMatrix(Model& model) const {
    auto locationId = glGetUniformLocation(m_programId, "ModelMatrix");
    auto transform = model.GetTransform();

    auto translation = glm::translate(glm::mat4(1.F), transform.GetTranslation());

    auto rotation = transform.GetRotation();
    auto rotationX = glm::mat4(1.F);
    auto rotationY = glm::mat4(1.F);
    auto rotationZ = glm::mat4(1.F);

    rotationX = glm::rotate(glm::mat4(1.F), glm::radians(rotation.x), glm::vec3(1.F, 0.F, 0.F));
    rotationY = glm::rotate(glm::mat4(1.F), glm::radians(rotation.y), glm::vec3(0.F, 1.F, 0.F));
    rotationZ = glm::rotate(glm::mat4(1.F), glm::radians(rotation.z), glm::vec3(0.F, 0.F, 1.F));

    glm::mat4 finalRotation = rotationX * rotationY * rotationZ;

    glm::mat4 scale = glm::scale(glm::mat4(1.F), transform.GetScale());

    const glm::mat4 modelMatrix = translation * finalRotation * scale;

    glUniformMatrix4fv(locationId, 1, GL_FALSE, &modelMatrix[0][0]);
}

void ShaderProgram::SetProjectionMatrix(const glm::mat4& projectionMatrix) {
    auto locationId = glGetUniformLocation(m_programId, "ProjectionMatrix");

    glUniformMatrix4fv(locationId, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void ShaderProgram::SetViewMatrix(const glm::mat4& viewMatrix) {
    auto locationId = glGetUniformLocation(m_programId, "ViewMatrix");

    glUniformMatrix4fv(locationId, 1, GL_FALSE, &viewMatrix[0][0]);
}

void ShaderProgram::Unbind() const {
    glUseProgram(0);
}