//
//  Window.hpp
//  QubGL
//
//  Copyright © 2020 Team FESTIVAL. All rights reserved.
//

#pragma once

#include <string>

class Window {
public:
    virtual unsigned int GetHeight() const = 0;
    virtual std::string GetTitle() const = 0;
    virtual unsigned int GetWidth() const = 0;

    virtual void Show() = 0;
};