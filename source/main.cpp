#include <iostream>
#include <vector>
#include <cstdint>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#include <stb/stb_image_write.h>

#include "vec3.h"

int main()
{
    const int width = 400;
    const int height = 400;
    const int channel = 3;

    std::cout << "start" << std::endl;

    std::vector<unsigned char> image_data(width * height * channel);
    int index = 0;
    for (int j = height - 1; j >= 0; --j) {
        for (int i = 0; i < width; ++i) {
            auto r = double(i) / (width - 1);
            auto g = double(j) / (height - 1);
            auto b = 0.25f;

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            image_data[index++] = (unsigned char)ir;
            image_data[index++] = (unsigned char)ig;
            image_data[index++] = (unsigned char)ib;
        }
    }

    stbi_write_bmp("out.bmp", width, height, 3, image_data.data());
    return 0;
}