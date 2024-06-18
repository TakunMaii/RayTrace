#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "ray.hpp"
#include "vec3.hpp"

class camera
{
public:
    vec3 origin = vec3(0, 0, 0);
    vec3 lower_left_corner = vec3(-2, -1, -1);
    vec3 horizontal = vec3(4, 0, 0);
    vec3 vertical = vec3(0, 2, 0);
    float f = 1.0;

    camera()
    {
        origin = vec3(0, 0, 0);
        lower_left_corner = vec3(-2, -1, -1);
        horizontal = vec3(4, 0, 0);
        vertical = vec3(0, 2, 0);
    }

    camera set_f(float f)
    {
        this->f = f;
        return *this;
    }

    camera(vec3 origin, vec3 lower_left_corner, vec3 horizontal, vec3 vertical) : origin(origin), lower_left_corner(lower_left_corner), horizontal(horizontal), vertical(vertical) {}

    camera(vec3 origin, vec3 eyedir, vec3 eyeup, float fov, float half_width, float half_height)
    {
        this->origin = origin;
        vec3 eyeleft = eyeup.cross(eyedir).normalize();
        this->lower_left_corner = origin + eyedir.normalize() * fov + eyeleft * half_width - eyeup * half_height;
        this->horizontal = (-eyeleft).normalize() * half_width * 2;
        this->vertical = eyeup.normalize() * half_height * 2;
    }

    ray get_ray(float u, float v)
    {
        vec3 forigin = origin + horizontal.normalize() * (random_float() * 2 - 1) * f + vertical.normalize() * (random_float() * 2 - 1) * f;
        return ray(forigin, lower_left_corner + horizontal * u + vertical * v - forigin);
    }
};

#endif