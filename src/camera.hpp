#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

class Camera{
        private:
            glm::vec3 position;
            glm::vec3 direction;

            glm::vec3 right;
            glm::vec3 up;
                   
            float yaw;   
            float pitch; 

            float mouseSensitivity = 0.18f;

            bool firstMouse;
            float lastX;
            float lastY;
        public:
            Camera(glm::vec3 pos, float yaw = 90.0f, float pitch = 0.0f);

            glm::mat4 viewMatrix();

            void setPos(glm::vec3 pos);
            glm::vec3 getPos();
            void move(glm::vec3 pos);

            void updateDirection();
            glm::vec3 getDir();
            glm::vec3 getRight();

            void processMouseMovement(float xoffset, float yoffset);

            static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
};