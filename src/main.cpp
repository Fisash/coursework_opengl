#include <iostream>  
#include <GL/glew.h>    // GLEW
#include <GLFW/glfw3.h> // GLFW

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "window.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "input.hpp"
#include "mesh.hpp"
#include "grid.hpp"

float speed = 2.5f;

const char* vertexShaderSource =  R"(#version 460 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aTexCoord;

        out vec2 TexCoord;
        out float FragDepth;
        out float h;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main()
        {
            vec4 worldPos = view * model * vec4(aPos, 1.0);
            h = aPos.y;
            gl_Position = projection * worldPos;
            TexCoord = aTexCoord;
            FragDepth = -worldPos.z; // Используем отрицательное значение, т.к. OpenGL использует правостороннюю систему координат
        })";

const char* fragmentShaderSource =  R"(#version 460 core    
        in vec2 TexCoord;  
        in float FragDepth;
        in float h;

        uniform sampler2D ourTexture;
        
        out vec4 FragColor;
 
        void main() {
            float brightness = clamp(1.0 - FragDepth * 0.1, 0.35, 0.9); // Чем дальше, тем темнее (но не полностью чёрное)
            vec4 texColor = texture(ourTexture, TexCoord);

            vec3 terrainColor;
            int texturingType = 1;

            // Определяем цвет в зависимости от высоты h
            if (h < 0.15) {
                // Ниже определенного уровня - синий (вода)
                terrainColor = vec3(0.1, 0.3, 1.0);
            } else if (h < 0.3) {
                // Ещё выше - желтый (пляж)
                texturingType = 0;
                terrainColor = vec3(0.8, 0.7, 0.3);
            } else if (h < 0.6) {
                // Затем зеленый (лес)
                texturingType = 0;
                terrainColor = vec3(0.2, 0.7, 0.15);
            } else if (h < 1.3) {
                // Потом серый (горы)
                terrainColor = vec3(0.28, 0.33, 0.4);
            } else {
                // И наконец белый (снег на вершинах)
                terrainColor = vec3(0.95, 1.0, 0.95);
            }
            
            vec3 baseColor;
            if(texturingType == 1)
                baseColor = (terrainColor + texColor.xyz);
            else if(texturingType == 0)
                baseColor = terrainColor + (0.3f*texColor.xyz);
            FragColor = vec4(baseColor*brightness, 1.0);
        })";

Window window(1000, 1000, "goida");
Camera mainCamera(glm::vec3(0.0f, 0.0f, -5.0f));
Shader shader(vertexShaderSource, fragmentShaderSource);

//Texture testTex("data\\tex.jpg");
Texture testTex = Texture(512, 512, 0.15f);

Mesh gridMesh(shader);

void render(){

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
    //model = glm::scale(model, glm::vec3(20.0f, 0.5f, 20.0f));
    shader.setMat4("model", model);

    gridMesh.render();

}

void input(float deltaTime){
    if(Input::getKey(GLFW_KEY_W))
        mainCamera.move(mainCamera.getDir()*speed*deltaTime);
    if(Input::getKey(GLFW_KEY_S))
        mainCamera.move(-mainCamera.getDir()*speed*deltaTime);
    if(Input::getKey(GLFW_KEY_D))
        mainCamera.move(mainCamera.getRight()*speed*deltaTime);
    if(Input::getKey(GLFW_KEY_A))
        mainCamera.move(-mainCamera.getRight()*speed*deltaTime);
    if(Input::getKey(GLFW_KEY_SPACE))
        mainCamera.move(glm::vec3(0.0f, speed*deltaTime, 0.0f));
    if(Input::getKey(GLFW_KEY_LEFT_SHIFT))
        mainCamera.move(glm::vec3(0.0f, -speed*deltaTime, 0.0f));
    if(Input::getKey(GLFW_KEY_LEFT_ALT))
        speed = 6.0f;
    if(Input::getKeyUp(GLFW_KEY_LEFT_ALT))
        speed = 2.5f;
}
int main() {

    glClearColor(0.2f, 0.25f, 0.35f, 1.0f); 
    glEnable(GL_DEPTH_TEST);

    testTex.bind();

    Grid grid(10, 10, 0.15f);
    gridMesh.setVertices(grid.genGridVertices(), false);
    gridMesh.setIndices(grid.genGridIndices());

    glfwSetWindowUserPointer(window.getGLFWWindowPtr(), &mainCamera);
    glfwSetCursorPosCallback(window.getGLFWWindowPtr(), Camera::mouseCallback);

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.15f, 100.0f);
    shader.use();
    shader.setMat4("projection", projection);

    float lastFrameTime = 0.0f;
    float deltaTime = 0.0f;

    while (!window.shouldClose()) {
        window.pollEvents();

        float time = (float)glfwGetTime();
        deltaTime = time - lastFrameTime;
        lastFrameTime = time;
        shader.setFloat("time", time);

        input(deltaTime);
        shader.setMat4("view", mainCamera.viewMatrix());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        render();

        window.swapBuffers();
        int err = glGetError();

        if (err != GL_NO_ERROR)
            std::cout<<err;
    }
    return 0;
}