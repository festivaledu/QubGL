#include <iostream>
#include <string>

#include "Rendering/Window.hpp"
#include "Rendering/WindowFactory.hpp"

int main(void) {
    Window* window = WindowFactory::CreateWindow("QubGL", 1920, 1080);
    window->Show();

    delete window;
    window = nullptr;

    return 0;
}