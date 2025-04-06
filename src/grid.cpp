#include "grid.hpp"
#include "stb_perlin.h"
#include <cstdlib> // для std::rand()
#include <ctime>   // для std::time()
#include "options.hpp"

Grid::Grid(unsigned int width, unsigned int height, float step)
        : width(width), height(height), step(step) {}

std::vector<float> Grid::genGridVertices() {
    std::srand(std::time(nullptr)); // Инициализация генератора случайных чисел
    float x_offset = static_cast<float>(std::rand()) / RAND_MAX * 1000.0f;
    float z_offset = static_cast<float>(std::rand()) / RAND_MAX * 1000.0f;

    // Вычисляем количество вершин вдоль каждой оси
    unsigned int numVerticesX = static_cast<unsigned int>(width / step) + 1;
    unsigned int numVerticesZ = static_cast<unsigned int>(height / step) + 1;

    std::vector<float> vertices;
    vertices.reserve(numVerticesX * numVerticesZ * 5);

    float scale = Options::scale;
    // Генерируем вершины: x, y, z, u, v
    for (float z = 0; z <= height; z += step) {
        for (float x = 0; x <= width; x += step) {
            vertices.push_back(x);

            float frequency = scale;
            float noise = 0.0f;
            float amplitude = 1.0f;
            int octaves = 7; // можно поиграться с количеством октав
            for (int i = 0; i < octaves; i++) {
                noise += amplitude * stb_perlin_noise3((x + x_offset) * frequency, 0, (z + z_offset) * frequency, 0, 0, 0);
                amplitude *= 0.5f;   // уменьшение амплитуды
                frequency *= 1.7f;   // увеличение частоты
            }
            float y = noise * 3.1f; // масштабирование высоты
            if(y < 0.145f)
                y = 0.145f;
            vertices.push_back(y);
            vertices.push_back(z);

            // Текстурные координаты (нормированные)
            float u = x / static_cast<float>(width);
            float v = z / static_cast<float>(height);
            vertices.push_back(u);
            vertices.push_back(v);
        }
    }
    return vertices;
}

std::vector<unsigned int> Grid::genGridIndices() {
    // Вычисляем количество вершин вдоль каждой оси
    unsigned int numVerticesX = static_cast<unsigned int>(width / step) + 1;
    unsigned int numVerticesZ = static_cast<unsigned int>(height / step) + 1;

    std::vector<unsigned int> indices;
    // Перебираем ячейки (квадратики)
    for (unsigned int z = 0; z < numVerticesZ - 1; ++z) {
        for (unsigned int x = 0; x < numVerticesX - 1; ++x) {
            // Вычисляем индексы вершин для квадрата
            unsigned int topLeft = z * numVerticesX + x;
            unsigned int topRight = topLeft + 1;
            unsigned int bottomLeft = (z + 1) * numVerticesX + x;
            unsigned int bottomRight = bottomLeft + 1;

            // Первый треугольник
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            // Второй треугольник
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }
    return indices;
}

