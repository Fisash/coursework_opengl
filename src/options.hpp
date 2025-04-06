#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "window.hpp"

class Options{
    public:
        static float scale;
        static int heigh;
        static int width;
        static bool isOnlyLines;

        static bool isShouldRegen;

        static void init(Window window);

        static void drawData();

        static void render();

        static void destroy();
};

float Options::scale = 0.2f;
int Options::heigh = 10;
int Options::width = 10;

bool Options::isOnlyLines = false;
bool Options::isShouldRegen = true;