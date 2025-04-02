#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
private:
    GLFWwindow* window;
    int width, height;
    const char* title;
public:
    Window(int w, int h, const char* t);

    ~Window();

    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();
    void getVersion(int& major, int& minor);
    GLFWwindow* getGLFWWindowPtr();
};
