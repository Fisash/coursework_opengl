#pragma once

#include <GL/glew.h>  
#include "shader.hpp"
#include <vector>

class Mesh{
    private:
        GLuint VAO, VBO, EBO;
        Shader* shaderPtr;

        unsigned int indexCount;

        void setAttributes();
    public:
        Mesh(Shader& shader);
        Mesh(const std::vector<float>& vertices, Shader& shader);
        void render();
        void setIndices(const std::vector<unsigned int>& indices);
        void setVertices(const std::vector<float>& vertices, bool updateIndeces = true);
};