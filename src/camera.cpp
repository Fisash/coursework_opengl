#include "camera.hpp"

Camera::Camera(glm::vec3 pos, float yaw, float pitch){
    position = pos;
    this->yaw = yaw;
    this->pitch = pitch;
    updateDirection();
}

glm::mat4 Camera::viewMatrix(){
    return glm::lookAt(position, 
            position+direction, 
            up);
}

glm::mat4 Camera::viewMatrix(glm::vec3 target){
    return glm::lookAt(position, 
            target, 
            up);
}

void Camera::setPos(glm::vec3 pos){position = pos;}
glm::vec3 Camera::getPos() {return position;}


void Camera::move(glm::vec3 pos){
    setPos(getPos() + pos);
}

glm::vec3 Camera::getDir(){
    return direction;
}
glm::vec3 Camera::getRight(){
    return right;
}
void Camera::updateDirection() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction = glm::normalize(newFront);

    right = glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f)));
    up = glm::normalize(glm::cross(right, direction));
}

void Camera::setDirection(float yaw, float pitch){
    this->yaw = yaw;
    this->pitch = pitch;
    updateDirection();
}

void Camera::processMouseMovement(float xoffset, float yoffset) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f) pitch = 89.0f;
    if(pitch < -89.0f) pitch = -89.0f;

    updateDirection();
}

void Camera::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
        // Получаем указатель на камеру из user pointer
        Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
        
        static float lastX = xpos;
        static float lastY = ypos;

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;

        camera->processMouseMovement(xoffset, yoffset);
    }