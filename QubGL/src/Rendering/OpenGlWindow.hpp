#pragma once

#include "Window.hpp"

struct GLFWwindow;

class OpenGlWindow : public Window {
public:
    OpenGlWindow(const std::string& title, unsigned int width, unsigned int height);

    virtual unsigned int GetHeight() const override;
    virtual std::string GetTitle() const override;
    virtual unsigned int GetWidth() const override;
	virtual void OpenGlWindow::ProcessInput(GLFWwindow* window);
    virtual void Show() override;

private:
    unsigned int m_height;
    std::string m_title;
    unsigned int m_width;
    GLFWwindow* m_window;
};