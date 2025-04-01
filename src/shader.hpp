#pragma once

#include <GL/glew.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
private:
    GLuint programID;
    void compileShader(GLuint shader, const char* source);

public:
    Shader(const char* vertexSource, const char* fragmentSource);
    void use() const;
    void setMat4(const char* name, glm::mat4 mat);
};