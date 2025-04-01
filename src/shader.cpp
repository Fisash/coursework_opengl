#include <GL/glew.h>
#include <iostream>
#include "shader.hpp"

void Shader::compileShader(GLuint shader, const char* source) {
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        throw std::runtime_error("Shader compilation failed: " + std::string(infoLog));
    }
}

Shader::Shader(const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    compileShader(vertexShader, vertexSource);
    compileShader(fragmentShader, fragmentSource);

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    int success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        throw std::runtime_error("Program linking failed: " + std::string(infoLog));
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() const { glUseProgram(programID); }

void Shader::setMat4(const char* name, glm::mat4 mat){
    int modelLoc = glGetUniformLocation(programID, name);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setFloat(const char* name, float val){
    int floatLoc = glGetUniformLocation(programID, name);
    glUniform1f(floatLoc, val);
}