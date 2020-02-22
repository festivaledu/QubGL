#include "OpenGlWindow.hpp"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>

#include "../Logic/Cube.hpp"
#include "Lighting/DirectionalLight.hpp"
#include "Loader.hpp"
#include "Model.hpp"
#include "ShaderProgram.hpp"

glm::vec3 m_cameraPosition = glm::vec3(0.F, 0.F, 5.F);
glm::vec3 m_cameraCenter = glm::vec3(0.F, 0.F, -1.F);
Cube cube(nullptr);

void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch (key) {
		case GLFW_KEY_W:
			m_cameraPosition.z -= .5F;
			break;
		case GLFW_KEY_S:
			m_cameraPosition.z += .5F;
			break;
		case GLFW_KEY_A:
			m_cameraPosition.x -= .5F;
			m_cameraCenter.x -= .5F;
			break;
		case GLFW_KEY_D:
			m_cameraPosition.x += .5F;
			m_cameraCenter.x += .5F;
			break;

		case GLFW_KEY_UP:
			m_cameraCenter.y += .5F;
			break;
		case GLFW_KEY_DOWN:
			m_cameraCenter.y -= .5F;
			break;
		case GLFW_KEY_LEFT:
			m_cameraCenter.x -= .5F;
			break;
		case GLFW_KEY_RIGHT:
			m_cameraCenter.x += .5F;
			break;
		}
	}
	
	if (action != GLFW_PRESS || cube.GetIsRotating()) return;

    auto d = ((mods && GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT) ? Direction::CounterClockwise : Direction::Clockwise;

    if (key == GLFW_KEY_B) {
        cube.Rotate(Side::Back, d);
    } else if (key == GLFW_KEY_F) {
        cube.Rotate(Side::Front, d);
    } else if (key == GLFW_KEY_L) {
        cube.Rotate(Side::Left, d);
    } else if (key == GLFW_KEY_R) {
        cube.Rotate(Side::Right, d);
    } else if (key == GLFW_KEY_T) {
        cube.Rotate(Side::Top, d);
    } else if (key == GLFW_KEY_U) {
        cube.Rotate(Side::Bottom, d);
    }

	switch (key) {
	/// Note: https://developer.valvesoftware.com/w/images/6/6c/Hammer_Axis_visual_guide.png
	// Cube controls
	case GLFW_KEY_KP_8:
		cube.Rotate(Axis::Y, Direction::Clockwise);
		break;
	case GLFW_KEY_KP_2:
		cube.Rotate(Axis::Y, Direction::CounterClockwise);
		break;
	case GLFW_KEY_KP_4:
		cube.Rotate(Axis::Z, Direction::Clockwise);
		break;
	case GLFW_KEY_KP_6:
		cube.Rotate(Axis::Z, Direction::CounterClockwise);
		break;

	// Cube section controls
	case GLFW_KEY_KP_7:
		cube.Rotate(Side::Left, Direction::Clockwise);
		break;
	case GLFW_KEY_KP_1:
		cube.Rotate(Side::Left, Direction::CounterClockwise);
		break;
	case GLFW_KEY_KP_9:
		cube.Rotate(Side::Right, Direction::Clockwise);
		break;
	case GLFW_KEY_KP_3:
		cube.Rotate(Side::Right, Direction::CounterClockwise);
		break;
	default: break;
	}
}

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

    glfwSetKeyCallback(m_window, OnKey);

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
    auto loaded = loader.LoadObjects("resources/Cube2.obj");

    Mesh mesh = loader.Meshes.at(0);
    
    cube.SetShader(&program);
    cube.GenerateModels(mesh);

    auto aspectRatio = 1920.F / 1080.F;

    glm::mat4 projection = glm::perspective(glm::radians(55.F), aspectRatio, .1F, 100.F);
    program.SetProjectionMatrix(projection);

    glEnable(GL_DEPTH_TEST);

    DirectionalLight dirLight;
    dirLight.Ambient = glm::vec4(.3F, .3F, .3F, 1.F);
    dirLight.Diffuse = glm::vec4(1.F, 1.F, 1.F, 1.F);
    dirLight.Direction = glm::vec3(0.F, -1.F, 0.F);
    dirLight.Specular = glm::vec4(0.F, 1.F, 0.F, 1.F);

    program.SetDirectionalLight(dirLight);

    while (!glfwWindowShouldClose(m_window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cube.Draw();

        glfwSwapBuffers(m_window);
        glfwPollEvents();

		glm::mat4 camera = glm::lookAt(m_cameraPosition, m_cameraCenter, glm::vec3(0.F, 1.F, 0.F));
		program.SetViewMatrix(camera);
    }

    glfwTerminate();
}
