#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera{
        private:
            glm::vec3 position;
            glm::vec3 cameraTarget;
            glm::vec3 cameraDirection;
            glm::vec3 right;
            glm::vec3 up;
        public:
            Camera(glm::vec3 pos);

            glm::mat4 viewMatrix();

            void setPos(glm::vec3 pos);
            glm::vec3 getPos();

            void setDirection(glm::vec3 dir);
};