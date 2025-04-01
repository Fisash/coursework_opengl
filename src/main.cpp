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
        layout (location = 1) in vec2 aTexCoord;

        out vec2 TexCoord;

        out vec3 vPos;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;


        void main()
        {
            vec4 viewPos = view * model * vec4(aPos, 1.0);
            vPos = viewPos.xyz;
            gl_Position = projection * viewPos;
            TexCoord = aTexCoord;
        })";

    const char* fragmentShaderSource =  R"(#version 330 core    
        in vec2 TexCoord;  
        in vec3 vPos;
        uniform sampler2D ourTexture;

        uniform float time;

        out vec4 FragColor;
 
        void main() {
            float distance = length(vPos);
            float brightness = clamp(distance / 3.5, 0.0, 1.0);
            brightness = 1.0 - brightness;
            FragColor = texture(ourTexture, TexCoord)*brightness;
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

    unsigned int indices[] = {
        0, 1, 3,  // Первый треугольник
        1, 2, 3   // Второй треугольник
    };

    Shader shader(vertexShaderSource, fragmentShaderSource);
    shader.use();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    testTex.bind();

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.5f)); 
        
    shader.setMat4("view", view);

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.15f, 100.0f);

    shader.setMat4("projection", projection);

    while (!window.shouldClose()) {
        float time = (float)glfwGetTime();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, time * glm::radians(210.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        shader.setFloat("time", time);
        shader.setMat4("model", model);


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        window.swapBuffers();
        window.pollEvents();
    }

    glfwTerminate();
    return 0;
}