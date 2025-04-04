#include <stb_image.h>
#include <iostream>
#include "texture.hpp"
#include "stb_perlin.h"


Texture::Texture(const char* source, GLenum wrap, GLenum filter){
    int width, height, nrChannels;
    unsigned char* data = stbi_load(source, &width, &height, &nrChannels, 0);
    if (!data) {
        throw std::runtime_error("Texture loading falled (no data): " + std::string(source));
    }
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

Texture::Texture(int width, int height, float scale, GLenum wrap, GLenum filter) {
    // Генерируем массив данных шума
    const int channels = 3; // RGB
    unsigned char* data = new unsigned char[width * height * channels];
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Вычисляем шум в диапазоне [-1.0, 1.0]
            float noise = stb_perlin_noise3(
                x * scale, 
                y * scale, 
                0.0f,      // Z-координата (можно анимировать)
                0, 0, 0    // Опции вращения (0 - по умолчанию)
            );
            
            // Нормализуем шум в [0, 255]
            auto value = static_cast<unsigned char>((noise + 1.0f) * 127.5f);
            
            // Заполняем RGB-каналы
            int index = (y * width + x) * channels;
            data[index]     = value; // R
            data[index + 1] = value; // G
            data[index + 2] = value; // B
        }
    }

    // Создаем OpenGL текстуру
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    // Параметры текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    // Загружаем данные
    glTexImage2D(
        GL_TEXTURE_2D, 
        0, 
        GL_RGB, 
        width, 
        height, 
        0, 
        GL_RGB, 
        GL_UNSIGNED_BYTE, 
        data
    );
    glGenerateMipmap(GL_TEXTURE_2D);

    // Очищаем данные
    delete[] data;
}
void Texture::bind(){
    glBindTexture(GL_TEXTURE_2D, ID);
}