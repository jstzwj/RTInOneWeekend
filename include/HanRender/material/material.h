#ifndef MATERIAL_H
#define MATERIAL_H

#include <HanRender/vec3.h>
#include <HanRender/ray.h>
#include <HanRender/hittable.h>

class material
{
public:
    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const = 0;
};

#endif