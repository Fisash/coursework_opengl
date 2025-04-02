#include "input.hpp"

void Input::setWindow(Window* w){window = w;}
bool Input::getKey(int keyCode){
    return (glfwGetKey(window->getGLFWWindowPtr(), keyCode) == GLFW_PRESS);
}
bool Input::getKeyUp(int keyCode){
    return (glfwGetKey(window->getGLFWWindowPtr(), keyCode) == GLFW_RELEASE);
}

Window* Input::window = nullptr;