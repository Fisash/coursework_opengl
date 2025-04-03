#include <vector>
#include <glm/glm.hpp>

class Grid {
private:
    unsigned int width;
    unsigned int height;
    float step;

public:
    Grid(unsigned int width, unsigned int height, float step);

    std::vector<float> genGridVertices();

    std::vector<unsigned int> genGridIndices();
};
