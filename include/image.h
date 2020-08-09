#ifndef IMAGE_H_
#define IMAGE_H_

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#include <stb/stb_image_write.h>

#include <cstdint>
#include <vector>

#include <vec3.h>

enum ImageType
{
    BMP,
    PNG,
    JPG
};

class Image
{
private:
    std::int32_t height;
    std::int32_t width;
    const std::int32_t channel = 3;
    std::vector<unsigned char> data;

public:
    Image(int height, int width)
        : height(height), width(width), data(width * height * channel)
    {
    }

    void set_color(int h, int w, color c)
    {
        h = height - 1 - h;
        data[h * width * channel + w * channel] = static_cast<unsigned char>(255.999 * c.x());
        data[h * width * channel + w* channel + 1] = static_cast<unsigned char>(255.999 * c.y());
        data[h * width * channel + w * channel + 2] = static_cast<unsigned char>(255.999 * c.z());
    }

    color get_color(int h, int w)
    {
        h = height - 1 - h;
        return color(
            data[h * width * channel + w * channel] / 255.f,
            data[h * width * channel + w * channel + 1] / 255.f,
            data[h * width * channel + w * channel + 2] / 255.f);
    }

    void write_to_file(const char *filename)
    {
        stbi_write_bmp(filename, width, height, channel, data.data());
    }
};

#endif