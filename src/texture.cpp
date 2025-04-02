#include <stb_image.h>
#include <iostream>
#include "texture.hpp"
#include "FastNoiseLite.h"
#include <vector>

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

Texture::Texture(float scale, GLenum wrap, GLenum filter) {
    const int width = 512;
    const int height = 512;
    // Будем хранить данные в формате RGB: 3 канала на пиксель.
    std::vector<unsigned char> noiseData(width * height * 3);

    // Инициализация генератора шума Перлина
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    // Параметр scale можно подобрать для достижения нужной детализации

    // Генерация шума и заполнение массива
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float n = noise.GetNoise(x * scale, y * scale);
            // Нормализация из диапазона [-1,1] в [0,1] и масштабирование до [0,255]
            unsigned char value = static_cast<unsigned char>(((n + 1.0f) * 0.5f) * 255);
            int index = (y * width + x) * 3;
            noiseData[index + 0] = value; // R
            noiseData[index + 1] = value; // G
            noiseData[index + 2] = value; // B
        }
    }

    // Создание и настройка текстуры OpenGL
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, noiseData.data());
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::bind(){
    glBindTexture(GL_TEXTURE_2D, ID);
}