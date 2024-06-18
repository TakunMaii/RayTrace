#ifndef RAYHPP

#define RAYHPP

#include "vec3.hpp"

class ray
{
public:
    vec3 origin, direction;

    ray() {}
    ray(const vec3 &origin, const vec3 &direction) : origin(origin), direction(direction) {}

    vec3 point_at_parameter(float t) const
    {
        return origin + direction * t;
    }
};

#endif