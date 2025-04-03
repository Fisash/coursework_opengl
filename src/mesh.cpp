#include "mesh.hpp"


void Mesh::setAttributes(){
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}


Mesh::Mesh(Shader& shader) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    this->shaderPtr = &shader;
} 

Mesh::Mesh(const std::vector<float>& vertices, Shader& shader) 
    : Mesh(shader) {
    setVertices(vertices);
}


void Mesh::render(){
    glBindVertexArray(VAO);
    shaderPtr->use();
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void Mesh::setIndices(const std::vector<unsigned int>& indices){
    indexCount = indices.size();
    glBindVertexArray(VAO); 

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void Mesh::setVertices(const std::vector<float>& vertices, bool updateIndeces){
    glBindVertexArray(VAO); 

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    if(updateIndeces){
        size_t vertexCount = vertices.size() / 5;
        std::vector<unsigned int> indices(vertexCount);
        for (unsigned int i = 0; i < vertexCount; ++i) {
            indices[i] = i;
        }
        setIndices(indices);
    }
    setAttributes();
}