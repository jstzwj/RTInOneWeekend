#include <iostream>
#include <vector>
#include <cstdint>

#include <vec3.h>
#include <image.h>

int main()
{
    const int width = 600;
    const int height = 400;

    std::cout << "start rendering" << std::endl;

    Image image(height, width);
    for (int j = height - 1; j >= 0; --j) {
        for (int i = 0; i < width; ++i) {
            auto r = static_cast<float>(i) / (width - 1);
            auto g = static_cast<float>(j) / (height - 1);
            auto b = 0.f;

            image.set_color(j, i, color(r, g, b));
        }
    }

    image.write_to_file("out.bmp");

    std::cout << "finish rendering" << std::endl;
    
    return 0;
}