#include "camera.hpp"

Camera::Camera(glm::vec3 pos){
    position = pos;
    cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    setDirection(glm::normalize(pos - cameraTarget));
}

glm::mat4 Camera::viewMatrix(){
    return glm::lookAt(position, 
            cameraTarget, 
            up);
}

void Camera::setPos(glm::vec3 pos){position = pos;}
glm::vec3 Camera::getPos() {return position;}

void Camera::setDirection(glm::vec3 dir){
    cameraDirection = dir;
    glm::vec3 globalUp = glm::vec3(0.0f, 1.0f, 0.0f); 
    right = glm::normalize(glm::cross(globalUp, cameraDirection));
    up = glm::cross(cameraDirection, right);
}