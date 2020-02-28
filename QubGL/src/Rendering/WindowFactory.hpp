//
//  WindowFactory.hpp
//  QubGL
//
//  Copyright © 2020 Team FESTIVAL. All rights reserved.
//

#pragma once

#include <string>

class Window;

class WindowFactory {
public:
    static Window* CreateWindow(const std::string& title, unsigned int width, unsigned int height);
};