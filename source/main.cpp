#include <iostream>
#include <vector>
#include <cstdint>
#include <memory>
#include <limits>

#include <indicators/indicators.hpp>

#include <util.h>
#include <vec3.h>
#include <ray.h>
#include <image.h>
#include <camera.h>
#include <hittable.h>
#include <sphere.h>
#include <hittable_list.h>

color ray_color(const ray &r, const hittable &world, int depth)
{
    hit_record rec;
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0, 0, 0);

    if (world.hit(r, 0, std::numeric_limits<double>::infinity(), rec))
    {
        point3 target = rec.p + rec.normal + random_in_unit_sphere();
        return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
    std::cout << "start rendering" << std::endl;

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 800;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;

    Image image(image_width, image_height);

    // world
    hittable_list world;
    world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Camera
    camera cam;

    // render
    using namespace indicators;
    ProgressBar bar{
        option::BarWidth{50},
        option::Start{"["},
        option::Fill{"="},
        option::Lead{">"},
        option::Remainder{" "},
        option::End{"]"},
        option::ShowPercentage{true},
        option::PostfixText{"Rendering"},
        option::ForegroundColor{Color::white},
        option::FontStyles{std::vector<FontStyle>{FontStyle::bold}}};
    int progress_value = 0;
    int progress_total = image_width * image_height;
    for (int j = image_height - 1; j >= 0; --j)
    {
        for (int i = 0; i < image_width; ++i)
        {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, 3);
            }
            image.set_color(i, j, pixel_color * (1.0 / samples_per_pixel));

            // update bar
            if (progress_value % (progress_total / 100) == 0)
                bar.set_progress((int)(progress_value * 100.f / progress_total));
            progress_value += 1;
        }
    }

    image.write_to_file("out.bmp");

    std::cout << "finish rendering" << std::endl;

    return 0;
}