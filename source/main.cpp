#include <iostream>
#include <vector>
#include <cstdint>
#include <memory>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <limits>

#include <indicators/indicators.hpp>
#include <taskflow/taskflow.hpp>

#include <util.h>
#include <vec3.h>
#include <ray.h>
#include <image.h>
#include <camera.h>
#include <hittable.h>
#include <sphere.h>
#include <hittable_list.h>
#include <material/lambertian.h>

color ray_color(const ray &r, const hittable &world, int depth)
{
    hit_record rec;
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0, 0, 0);

    if (world.hit(r, 0.001, std::numeric_limits<double>::infinity(), rec))
    {
        point3 target = rec.p + rec.normal + random_unit_vector();
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
    const int samples_per_pixel = 1000;
    const float gamma = 2.f;

    Image image(image_width, image_height);

    // materials

    auto lambertian_mat = std::make_shared<lambertian>(color(1, 0, 0));

    // world
    hittable_list world;
    world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5, lambertian_mat));
    world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100, lambertian_mat));

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
        option::ShowElapsedTime{true},
        option::ShowRemainingTime{true},
        option::PostfixText{"Rendering"},
        option::ForegroundColor{Color::white},
        option::FontStyles{std::vector<FontStyle>{FontStyle::bold}}};

    // taskflow
    tf::Executor executor;

    int progress_value = 0;
    int progress_total = image_width * image_height;
    for (int j = image_height - 1; j >= 0; --j)
    {
        for (int i = 0; i < image_width; ++i)
        {
            std::vector<color> pixel_colors(samples_per_pixel, color(0, 0, 0));
            tf::Taskflow taskflow;

            auto [S, T] = taskflow.parallel_for(
                0, samples_per_pixel, 1,
                [&](int s) {
                    auto u = (i + random_double()) / (image_width - 1);
                    auto v = (j + random_double()) / (image_height - 1);
                    ray r = cam.get_ray(u, v);
                    pixel_colors[s] += ray_color(r, world, 3);
                },
                4  // at least two items at a time
            );

            executor.run(taskflow).wait();

            color pixel_color = std::accumulate(pixel_colors.begin(), pixel_colors.end(), color(0, 0, 0));
            

            /*
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, 3);
            }
            */

            color avg_pixel_color = pixel_color * (1.0 / samples_per_pixel);
            color gamma_pixel_color = color(
                std::pow(avg_pixel_color.x(), 1.f / gamma),
                std::pow(avg_pixel_color.y(), 1.f / gamma),
                std::pow(avg_pixel_color.z(), 1.f / gamma)
            );
            image.set_color(i, j, gamma_pixel_color);

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