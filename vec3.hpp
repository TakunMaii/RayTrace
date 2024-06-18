// vector3 class

#ifndef VEC3_HPP
#define VEC3_HPP

#include <iostream>
#include <math.h>
#include <stdlib.h>

using namespace std;

int initial_rand(int seed) {
    srand(seed);
    return seed;
}

// from 0 to 1
float random_float()
{
    static int seed = initial_rand((unsigned long long) new char);
    float r =  rand() / (RAND_MAX + 1.0);
    if(r!=r)printf("nan in rand()");
    return r;
}


class vec3
{
public:
    float x, y, z;

    vec3() : x(0), y(0), z(0) {}
    vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    vec3 operator+(const vec3 &v) const
    {
        return vec3(x + v.x, y + v.y, z + v.z);
    }

    vec3 operator-(const vec3 &v) const
    {
        return vec3(x - v.x, y - v.y, z - v.z);
    }

    vec3 operator*(const vec3 &v) const
    {
        return vec3(x * v.x, y * v.y, z * v.z);
    }

    vec3 operator/(const vec3 &v) const
    {
        return vec3(x / v.x, y / v.y, z / v.z);
    }

    vec3 operator*(float t) const
    {
        return vec3(x * t, y * t, z * t);
    }

    vec3 operator/(float t) const
    {
        return vec3(x / t, y / t, z / t);
    }

    vec3 operator-() const
    {
        return vec3(-x, -y, -z);
    }

    float length() const
    {
        return sqrt(x * x + y * y + z * z);
    }

    float squared_length() const
    {
        return x * x + y * y + z * z;
    }

    vec3 normalize() const
    {
        float len = length();
        if(len == 0)printf("zero length vector\n");
        return vec3(x / len, y / len, z / len);
    }

    float dot(const vec3 &v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }

    vec3 cross(const vec3 &v) const
    {
        return vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }

    void limit()
    {
        x = x > 1 ? 1 : x;
        y = y > 1 ? 1 : y;
        z = z > 1 ? 1 : z;
        x = x < 0 ? 0 : x;
        y = y < 0 ? 0 : y;
        z = z < 0 ? 0 : z;
    }

};
    vec3 random_in_unit_sphere()
    {
        vec3 p;
        do
        {
            p = vec3(random_float(), random_float(),random_float()) * 2 - vec3(1, 1, 1);
        } while (p.squared_length() > 1.0);
        return p;
    }

    vec3 random_positive_to(vec3 normal)
    {
        vec3 p;
        do
        {
            p = vec3(random_float(), random_float(), random_float()) * 2 - vec3(1, 1, 1);
        } while (p.dot(normal) < 0);
        return p.normalize();
    }

#endif