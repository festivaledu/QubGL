#include <iostream>
#include <string>

#include "Rendering/Window.hpp"
#include "Rendering/WindowFactory.hpp"

int main(void) {
	std::cout << "Hello World!" << std::endl;

	Window* window = WindowFactory::CreateWindow("Jonas tinkt", 1920, 1080);
	window->Show();

	delete window;
	window = nullptr;

	return 0;
}