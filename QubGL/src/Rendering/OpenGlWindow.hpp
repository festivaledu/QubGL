#pragma once

#include "Window.hpp"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"
#include "GLM/glm.hpp"

struct GLFWwindow;

class OpenGlWindow : public Window {
public:
    OpenGlWindow(const std::string& title, unsigned int width, unsigned int height);

    virtual unsigned int GetHeight() const override;
    virtual std::string GetTitle() const override;
    virtual unsigned int GetWidth() const override;
    virtual void Show() override;

private:
    unsigned int m_height;
    std::string m_title;
    unsigned int m_width;
    GLFWwindow* m_window;
};