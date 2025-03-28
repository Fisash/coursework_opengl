#include <iostream>  
#include <GL/glew.h>    // GLEW
#include <GLFW/glfw3.h> // GLFW

#include "window.hpp"
#include "shader.hpp"

int main() {
    
    const char* vertexShaderSource =  R"(#version 330 core
    layout (location = 0) in vec3 aPos;
    void main(){
     gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    })";

    const char* fragmentShaderSource =  R"(#version 330 core
    out vec4 FragColor;
    void main()
    {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    })";

    Window window(800, 600, "OpenGL + GLEW");

    float vertices[] = {
        0.5f,  0.5f, 0.0f,  // Верхняя правая
        0.5f, -0.5f, 0.0f,  // Нижняя правая
        -0.5f, -0.5f, 0.0f,  // Нижняя левая
        -0.5f,  0.5f, 0.0f   // Верхняя левая
    };

    unsigned int indices[] = {
        0, 1, 3,  // Первый треугольник
        1, 2, 3   // Второй треугольник
    };

    Shader shader(vertexShaderSource, fragmentShaderSource);
    shader.use();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 

    // Настройка VAO, VBO, EBO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Главный цикл
    while (!window.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        window.swapBuffers();
        window.pollEvents();
    }

    glfwTerminate();
    return 0;
}