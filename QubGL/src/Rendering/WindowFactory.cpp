//
//  WindowFactory.cpp
//  QubGL
//
//  Copyright © 2020 Team FESTIVAL. All rights reserved.
//

#include "WindowFactory.hpp"

#include "OpenGlWindow.hpp"

Window* WindowFactory::CreateWindow(const std::string& title, unsigned int width, unsigned int height)
{
    return new OpenGlWindow(title, width, height);
}