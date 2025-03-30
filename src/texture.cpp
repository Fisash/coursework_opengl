#include <stb_image.h>
#include <iostream>
#include "texture.hpp"

Texture::Texture(const char* source, GLenum wrap, GLenum filter){
    int width, height, nrChannels;
    unsigned char* data = stbi_load(source, &width, &height, &nrChannels, 0);
    if (!data) {
        throw std::runtime_error("Texture loading falled (file not found): " + std::string(source));
    }
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

void Texture::bind(){
    glBindTexture(GL_TEXTURE_2D, ID);
}