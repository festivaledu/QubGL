#include "OpenGlWindow.hpp"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>

#include "Lighting/DirectionalLight.hpp"
#include "Loader.hpp"
#include "Model.hpp"
#include "ShaderProgram.hpp"

OpenGlWindow::OpenGlWindow(const std::string& title, unsigned int width, unsigned int height)
    :m_title(title), m_width(width), m_height(height), m_window(nullptr) {

    if (!glfwInit()) return;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    if (!m_window) {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);

    auto initResult = glewInit();
    if (initResult != GLEW_OK) {
        fprintf(stderr, "An error occured: %s\n", glewGetErrorString(initResult));
    }

    std::cout << "Open GL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;

    glClearColor(0.F, 0.F, 0.F, 1.F);
}

unsigned int OpenGlWindow::GetHeight() const {
    return m_height;
}

std::string OpenGlWindow::GetTitle() const {
    return m_title;
}

unsigned int OpenGlWindow::GetWidth() const {
    return m_width;
}

void OpenGlWindow::Show() {
    ShaderProgram program("resources/BasicVertex.shader", "resources/BasicFragment.shader");
    program.Bind();

    Loader loader;
    auto loaded = loader.ParseFile("resources/simple_cube.obj");

    Model model(loader.GetMeshes()[0]);

    auto aspectRatio = 1920.F / 1080.F;

    glm::mat4 projection = glm::perspective(glm::radians(55.F), aspectRatio, .1F, 100.F);
    program.SetProjectionMatrix(projection);

    glm::mat4 camera = glm::lookAt(glm::vec3(0.F, 0.F, 5.F), glm::vec3(0.F, 0.F, -1.F), glm::vec3(0.F, 1.F, 0.F));
    program.SetViewMatrix(camera);

    glEnable(GL_DEPTH_TEST);

    DirectionalLight dirLight;
    dirLight.Ambient = glm::vec4(.3F, .3F, .3F, 1.F);
    dirLight.Diffuse = glm::vec4(1.F, 1.F, 1.F, 1.F);
    dirLight.Direction = glm::vec3(0.F, -1.F, 0.F);
    dirLight.Specular = glm::vec4(0.F, 1.F, 0.F, 1.F);

    program.SetDirectionalLight(dirLight);

    auto angleY = 0.F;

    while (!glfwWindowShouldClose(m_window)) {
        angleY += .05F;

        if (angleY > 360) {
            angleY = 0;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        model.Draw(program);

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    glfwTerminate();
}