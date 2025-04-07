#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "window.hpp"

namespace Options{
        extern float scale;
        extern float detailiztion;
        extern int texDetailiztion;
        extern int heigh;
        extern int width;
        extern bool isOnlyLines;

        extern bool isShouldRegen;

        void init(Window* window);

        void drawData();

        void render();

        void destroy();
};