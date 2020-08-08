#include <iostream>
#include <vector>
#include <cstdint>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#include <stb/stb_image_write.h>

int main()
{
    const int width = 400;
    const int height = 400;

    std::cout << "start" << std::endl;

    std::vector<std::uint32_t> image_data(width * height);
    for (int j = height - 1; j >= 0; --j) {
        for (int i = 0; i < width; ++i) {
            auto r = double(i) / (width - 1);
            auto g = double(j) / (height - 1);
            auto b = 0.25;

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);
            int ia = 0;

            image_data[j * width + i] = (ir << 24) | (ig << 16) | (ib << 8) | ia;
        }
    }

    stbi_write_bmp("out.bmp", width, height, 4, image_data.data());
    return 0;
}