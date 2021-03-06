//
//  OpenGlWindow.cpp
//  QubGL
//
//  Copyright © 2020 Team FESTIVAL. All rights reserved.
//

#include "OpenGlWindow.hpp"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>

#include "../Logic/Cube.hpp"
#include "Lighting/DirectionalLight.hpp"
#include "Loader.hpp"
#include "Model.hpp"
#include "ShaderProgram.hpp"

bool m_catchMouse = true;

glm::vec3 m_cameraPosition = glm::vec3(0.F, 0.F,  5.F);
glm::vec3 m_cameraFront = glm::vec3(0.F, 0.F, -1.F);
glm::vec3 m_cameraUp = glm::vec3(0.F, 1.F,  0.F);

bool m_mouseDidMove = false;
double m_cameraSensitivity = .15F;
double m_cameraYaw = -90.F;
double m_cameraPitch = 0.F;
double m_cameraPreviousX = 800.F / 2.F;
double m_cameraPreviousY = 600.F / 2.F;
float m_cameraFieldOfView = 55.F;

Cube cube(nullptr);

void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE) glfwDestroyWindow(window);

	// Prevent actions when the cube is still rotating
	if (action != GLFW_PRESS || cube.GetIsRotating()) return;

	// Change direction of rotation when Ctrl is pressed
    auto d = ((mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL) ? Direction::CounterClockwise : Direction::Clockwise;

	switch (key) {
	case GLFW_KEY_KP_0:
	case GLFW_KEY_N:
		cube.Rotate(Side::Front, Direction::CounterClockwise);
		break;
	case GLFW_KEY_KP_DECIMAL:
	case GLFW_KEY_M:
		cube.Rotate(Side::Front, Direction::Clockwise);
		break;
	case GLFW_KEY_KP_DIVIDE:
	case GLFW_KEY_7:
		cube.Rotate(Side::Back, Direction::CounterClockwise);
		break;
	case GLFW_KEY_KP_MULTIPLY:
	case GLFW_KEY_8:
		cube.Rotate(Side::Back, Direction::Clockwise);
		break;
	case GLFW_KEY_KP_8:
	case GLFW_KEY_U:
		cube.Rotate(Side::Top, d);
		break;
	case GLFW_KEY_KP_2:
	case GLFW_KEY_J:
		cube.Rotate(Side::Bottom, d);
		break;
	case GLFW_KEY_KP_4:
	case GLFW_KEY_H:
		cube.Rotate(Side::Left, d);
		break;
	case GLFW_KEY_KP_6:
	case GLFW_KEY_K:
		cube.Rotate(Side::Right, d);
		break;

	case GLFW_KEY_R:
		m_cameraFieldOfView = 55.F;
		break;
	default: break;
	}
}

void OnMouseKey(GLFWwindow* window, int button, int action, int mods) {
	if (action != GLFW_PRESS) return;
	
	switch (button) {
	case GLFW_MOUSE_BUTTON_MIDDLE:
		m_catchMouse = !m_catchMouse;
		glfwSetInputMode(window, GLFW_CURSOR, m_catchMouse ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
		glfwGetCursorPos(window, &m_cameraPreviousX, &m_cameraPreviousY);
		break;
	default: break;
	}
}

void OnMouseMove(GLFWwindow* window, double xPos, double yPos) {
	if (!m_catchMouse) return;

	if (!m_mouseDidMove)
    {
        m_cameraPreviousX = xPos;
        m_cameraPreviousY = yPos;
        m_mouseDidMove = true;
    }
	
	double xOffset = xPos - m_cameraPreviousX;
    double yOffset = m_cameraPreviousY - yPos;
    m_cameraPreviousX = xPos;
    m_cameraPreviousY = yPos;

    xOffset *= m_cameraSensitivity;
    yOffset *= m_cameraSensitivity;

    m_cameraYaw += xOffset;
    m_cameraPitch += yOffset;

    if (m_cameraPitch > 89.F) m_cameraPitch = 89.F;
    if (m_cameraPitch < -89.F) m_cameraPitch = -89.F;

    glm::vec3 front;
    front.x = cos(glm::radians(m_cameraYaw)) * cos(glm::radians(m_cameraPitch));
    front.y = sin(glm::radians(m_cameraPitch));
    front.z = sin(glm::radians(m_cameraYaw)) * cos(glm::radians(m_cameraPitch));
    m_cameraFront = glm::normalize(front);
}

void OnMouseScroll(GLFWwindow* window, double xOffset, double yOffset) {	
	m_cameraFieldOfView -= yOffset;
	if (m_cameraFieldOfView < 0.1F) m_cameraFieldOfView = 0.1F;
	if (m_cameraFieldOfView > 175.F) m_cameraFieldOfView = 175.F;
}

OpenGlWindow::OpenGlWindow(const std::string& title, unsigned int width, unsigned int height)
    :m_title(title), m_width(width), m_height(height), m_window(nullptr) {

    if (!glfwInit()) return;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

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
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    glfwSetKeyCallback(m_window, OnKey);
	glfwSetCursorPosCallback(m_window, OnMouseMove);
	glfwSetMouseButtonCallback(m_window, OnMouseKey);
	glfwSetScrollCallback(m_window, OnMouseScroll);

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

void OpenGlWindow::ProcessInput(GLFWwindow* window) {
	float cameraSpeed = .25F;

	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
		cameraSpeed = .1F;
	} else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		cameraSpeed = .75F;
	}

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        m_cameraPosition += cameraSpeed * m_cameraFront;
	}

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        m_cameraPosition -= cameraSpeed * m_cameraFront;
	}

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        m_cameraPosition -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
	}

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        m_cameraPosition += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
	}
}

void OpenGlWindow::Show() {
    ShaderProgram program("resources/BasicVertex.shader", "resources/BasicFragment.shader");
    program.Bind();

	// Initialize loader and load cube from obj
    Loader loader;
    auto loaded = loader.LoadObjects("resources/Cube2.obj");

	// Get loaded mesh
    Mesh mesh = loader.Meshes.at(0);
    
    cube.SetShader(&program);

	// Generate cube formation
    cube.GenerateModels(mesh);

    auto aspectRatio = 1920.F / 1080.F;

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);

    DirectionalLight dirLight;
    dirLight.Ambient = glm::vec4(.3F, .3F, .3F, 1.F);
    dirLight.Diffuse = glm::vec4(1.F, 1.F, 1.F, 1.F);
    dirLight.Direction = glm::vec3(0.F, -1.F, 0.F);
    dirLight.Specular = glm::vec4(0.F, 1.F, 0.F, 1.F);

    program.SetDirectionalLight(dirLight);

    while (!glfwWindowShouldClose(m_window)) {
		ProcessInput(m_window);
		
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(m_cameraFieldOfView), aspectRatio, .1F, 100.F);
		program.SetProjectionMatrix(projection);
		
		glm::mat4 camera = glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraFront, m_cameraUp);
		program.SetViewMatrix(camera);

        cube.Draw();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    glfwTerminate();
}
