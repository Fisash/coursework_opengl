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


int main() {
    
    const char* vertexShaderSource =  R"(#version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aTexCoord;

        out vec2 TexCoord;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;


        void main()
        {
            gl_Position = projection * view * model * vec4(aPos, 1.0);;
            TexCoord = aTexCoord;
        })";

    const char* fragmentShaderSource =  R"(#version 330 core    
        in vec2 TexCoord;  
        uniform sampler2D ourTexture;

        uniform float time;

        out vec4 FragColor;
 
        void main() {
            FragColor = texture(ourTexture, TexCoord);
        })";

    Window window(1000, 1000, "goida");

   float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f
    };

    Shader shader(vertexShaderSource, fragmentShaderSource);
    shader.use();

    glClearColor(0.3f, 0.4f, 0.5f, 0.8f); 
    glEnable(GL_DEPTH_TEST);

    Texture testTex("data\\tex.jpg");

    // Настройка VAO, VBO, EBO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    testTex.bind();
    Camera mainCamera(glm::vec3(0.0f, 0.0f, -5.0f));

    glfwSetWindowUserPointer(window.getGLFWWindowPtr(), &mainCamera);
    glfwSetCursorPosCallback(window.getGLFWWindowPtr(), Camera::mouseCallback);

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.15f, 100.0f);

    shader.setMat4("projection", projection);

    float lastFrameTime = 0.0f;
    float deltaTime = 0.0f;

    float speed = 2.0f;
    while (!window.shouldClose()) {
        float time = (float)glfwGetTime();
        deltaTime = time - lastFrameTime;
        lastFrameTime = time;

        shader.setFloat("time", time);

        //mainCamera.setDirection(glm::vec3(sin(time), 0.0f, 0.0f));
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
            speed = 2.0f;
        
        shader.setMat4("view", mainCamera.viewMatrix());

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::sin(time), glm::vec3(0.3f, 0.5f, 1.0f));
        shader.setMat4("model", model);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        window.swapBuffers();
        window.pollEvents();
    }
    return 0;
}