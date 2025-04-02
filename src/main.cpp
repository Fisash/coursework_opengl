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

float speed = 2.5f;

const char* vertexShaderSource =  R"(#version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aTexCoord;

        out vec2 TexCoord;
        out float FragDepth;
        
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main()
        {
            vec4 worldPos = view * model * vec4(aPos, 1.0);
            gl_Position = projection * worldPos;
            TexCoord = aTexCoord;
            FragDepth = -worldPos.z; // Используем отрицательное значение, т.к. OpenGL использует правостороннюю систему координат
        })";

const char* fragmentShaderSource =  R"(#version 330 core    
        in vec2 TexCoord;  
        in float FragDepth;
        
        uniform sampler2D ourTexture;
        
        out vec4 FragColor;
 
        void main() {
            float brightness = clamp(1.0 - FragDepth * 0.1, 0.2, 1.0); // Чем дальше, тем темнее (но не полностью чёрное)
            vec4 texColor = texture(ourTexture, TexCoord);
            FragColor = vec4(texColor.rgb * brightness, texColor.a);
        })";


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

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

Window window(1000, 1000, "goida");
Camera mainCamera(glm::vec3(0.0f, 0.0f, -5.0f));
Shader shader(vertexShaderSource, fragmentShaderSource);
//Texture testTex("data\\tex.jpg");
Texture testTex = Texture(0.8f);


void render(){
    for(unsigned int i = 0; i < 10; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i; 
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        shader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
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
    
    shader.use();

    glClearColor(0.3f, 0.4f, 0.5f, 0.8f); 
    glEnable(GL_DEPTH_TEST);

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

    glfwSetWindowUserPointer(window.getGLFWWindowPtr(), &mainCamera);
    glfwSetCursorPosCallback(window.getGLFWWindowPtr(), Camera::mouseCallback);

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.15f, 100.0f);

    shader.setMat4("projection", projection);

    float lastFrameTime = 0.0f;
    float deltaTime = 0.0f;

    while (!window.shouldClose()) {
        float time = (float)glfwGetTime();
        deltaTime = time - lastFrameTime;
        lastFrameTime = time;
        shader.setFloat("time", time);

        input(deltaTime);
        shader.setMat4("view", mainCamera.viewMatrix());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        render();

        window.swapBuffers();
        window.pollEvents();
    }
    return 0;
}