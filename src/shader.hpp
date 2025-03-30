#pragma once

#include <GL/glew.h>
#include <iostream>

class Shader {
private:
    GLuint programID;
    void compileShader(GLuint shader, const char* source);

public:
    Shader(const char* vertexSource, const char* fragmentSource);
    void use() const;
};