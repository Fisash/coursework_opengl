#include <GL/glew.h>
#include <iostream>
#include "window.hpp"
#include "input.hpp"

#define MAJOR_VERSION 4
#define MINOR_VERSION 6 

Window::Window(int w, int h, const char* t) : width(w), height(h), title(t) 
{
    if (!glfwInit()) {
        throw std::runtime_error("GLFW initialization failed!");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Window creation failed!");
    }

    glfwMakeContextCurrent(window);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    Input::setWindow(this);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        throw std::runtime_error("GLEW initialization failed!");
    }
}

Window::~Window() {
    if(window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void Window::swapBuffers() {
    glfwSwapBuffers(window);
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::getVersion(int& major, int& minor){
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
}

GLFWwindow* Window::getGLFWWindowPtr(){
    return window;
}