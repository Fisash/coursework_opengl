#include <iostream>  
#include <GL/glew.h>    // GLEW
#include <GLFW/glfw3.h> // GLFW

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "window.hpp"
#include "shader.hpp"
#include "texture.hpp"

int main() {
    
    const char* vertexShaderSource =  R"(#version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        layout (location = 2) in vec2 aTexCoord;

        out vec3 ourColor;
        out vec2 TexCoord;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main()
        {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
            ourColor = aColor;
            TexCoord = aTexCoord;
        })";

    const char* fragmentShaderSource =  R"(#version 330 core
        in vec3 ourColor;     
        in vec2 TexCoord;  
        uniform sampler2D ourTexture;
        out vec4 FragColor;

        void main() {
            FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
        })";

    Window window(1000, 1000, "goida");

   float vertices[] = {
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.5f, 0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.5f, 1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.5f, 0.0f, 1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.5f, 0.0f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 1.0f,

        // Верхняя грань (голубой)
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 3,  // Первый треугольник
        1, 2, 3   // Второй треугольник
    };

    Shader shader(vertexShaderSource, fragmentShaderSource);
    shader.use();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 


    Texture testTex("data\\tex.jpg");

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


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    testTex.bind();

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.5f)); 
        
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(35.0f), 900.0f / 600.0f, 0.15f, 100.0f);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    while (!window.shouldClose()) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        
        shader.setMat4("model", model);

        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        window.swapBuffers();
        window.pollEvents();
    }

    glfwTerminate();
    return 0;
}