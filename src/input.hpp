#pragma once

#include "window.hpp"
#include <functional>

class Input{
    private:
        static Window* window;
    public:
        static void setWindow(Window* w);
        static bool getKey(int keyCode);
        static bool getKeyUp(int keyCode);
};