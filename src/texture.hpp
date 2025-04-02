#pragma once

#include <GL/glew.h>

class Texture{
    private:
        GLuint ID;
    public:
        Texture(const char* source, GLenum wrap = GL_REPEAT, GLenum filter = GL_NEAREST);
        Texture(float scale, GLenum wrap = GL_REPEAT, GLenum filter = GL_NEAREST);
        void bind();
};