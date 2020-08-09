#ifndef MATERIAL_LAMBERTIAN_H_
#define MATERIAL_LAMBERTIAN_H_

#include <hittable.h>
#include <ray.h>
#include <vec3.h>
#include <material/material.h>

class lambertian : public material
{
public:
    lambertian(const color &a) : albedo(a) {}

    virtual bool scatter(
        const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
    {
        vec3 scatter_direction = rec.normal + random_unit_vector();
        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

public:
    color albedo;
};

#endif