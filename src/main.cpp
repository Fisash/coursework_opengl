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

#include "options.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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


Window*  window = nullptr;
Shader* shader = nullptr;
Camera* mainCamera = nullptr;
Mesh*  gridMesh = nullptr;

glm::vec3 terrainCenter(0, 0, 0); 

void render(){

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
    //model = glm::scale(model, glm::vec3(20.0f, 0.5f, 20.0f));
    shader->setMat4("model", model);
    gridMesh->render();

}

void input(float deltaTime){
    if(Input::getKey(GLFW_KEY_W))
        mainCamera->move(mainCamera->getDir()*speed*deltaTime);
    if(Input::getKey(GLFW_KEY_S))
        mainCamera->move(-mainCamera->getDir()*speed*deltaTime);
    if(Input::getKey(GLFW_KEY_D))
        mainCamera->move(mainCamera->getRight()*speed*deltaTime);
    if(Input::getKey(GLFW_KEY_A))
        mainCamera->move(-mainCamera->getRight()*speed*deltaTime);
    if(Input::getKey(GLFW_KEY_SPACE))
        mainCamera->move(glm::vec3(0.0f, speed*deltaTime, 0.0f));
    if(Input::getKey(GLFW_KEY_LEFT_SHIFT))
        mainCamera->move(glm::vec3(0.0f, -speed*deltaTime, 0.0f));
    if(Input::getKey(GLFW_KEY_LEFT_ALT))
        speed = 6.0f;
    if(Input::getKeyUp(GLFW_KEY_LEFT_ALT))
        speed = 2.5f;
}

void generateGrid(Mesh *&mesh){
    Grid grid(Options::width, Options::heigh, 1/Options::detailiztion);
    terrainCenter = glm::vec3(Options::width/2, -3.0f,Options::heigh/2); 
    mesh = new Mesh(grid.genGridVertices(), *shader);
    mesh->setIndices(grid.genGridIndices());
}

int main() {
    window = new Window(1000, 1000, "goida");
    shader = new Shader(vertexShaderSource, fragmentShaderSource);
    mainCamera = new Camera(glm::vec3(0.0f, 3.0f, -5.0f));

    //Texture testTex("data\\tex.jpg");
    Texture testTex = Texture(512, 512, 0.15f);


    glClearColor(0.2f, 0.25f, 0.35f, 1.0f); 
    glEnable(GL_DEPTH_TEST);

    testTex.bind();

    generateGrid(gridMesh);

    //glfwSetWindowUserPointer(window.getGLFWWindowPtr(), &mainCamera);
    //glfwSetCursorPosCallback(window.getGLFWWindowPtr(), Camera::mouseCallback);

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(60.0f), 1.0f, 0.15f, 100.0f);
    shader->use();
    shader->setMat4("projection", projection);

    float lastFrameTime = 0.0f;
    float deltaTime = 0.0f;
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    Options::init(window);

    while (!window->shouldClose()) {
        window->pollEvents();

        float time = (float)glfwGetTime();
        deltaTime = time - lastFrameTime;
        lastFrameTime = time;
        shader->setFloat("time", time);

        if(Options::isShouldRegen){
            generateGrid(gridMesh);
            Options::isShouldRegen = false;
        }
        if(mainCamera->interactMovementMode){
            input(deltaTime); 
            shader->setMat4("view", mainCamera->viewMatrix());
        }
        else{
            float radius = 10.0f; 
            float height = 12.0f;

            float camX = terrainCenter.x + sin(time * 0.5f) * radius;
            float camZ = terrainCenter.z + cos(time * 0.5f) * radius; 
            mainCamera->setPos(glm::vec3(camX, terrainCenter.y + height, camZ)); 
            shader->setMat4("view", mainCamera->viewMatrix(terrainCenter));
        }

        Options::render();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        render();
        Options::drawData();
        
        window->swapBuffers();
        int err = glGetError();

        if (err != GL_NO_ERROR)
            std::cout<<err;
    }
    Options::destroy();
    delete window;
    delete shader;
    delete mainCamera;
    delete gridMesh;

    return 0;
}