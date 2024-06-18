#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include "vec3.hpp"
#include "ray.hpp"

class material
{
public:
    vec3 color;
    vec3 emission_color = vec3(1, 1, 1);
    float emission_strength = 0;
    float reflection_strength = 0;
    float attenuation = 0;
    float refraction_strength = 0;
    float refraction_index = 1.5;

    material() : color(vec3(0, 0, 0)), emission_color(vec3(0, 0, 0)) {}
    material(vec3 color, float reflection_strength, float attenuation) : color(color), reflection_strength(reflection_strength), attenuation(attenuation) {
    }
    material set_refrection(float refraction, float index = 1.5)
    {
        refraction_strength = refraction;
        refraction_index = index;
        return *this;
    }
    material set_emission(vec3 emission, float strength)
    {
        emission_color = emission;
        emission_strength = strength;
        return *this;
    }
};

class hit_record
{
public:
    float t;
    vec3 p;
    vec3 normal;
    vec3 income;
    material mat;
};

class hitable
{
public:
    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const = 0;
};

class plane : public hitable
{
public:
    vec3 normal;
    float d;
    material mat;

    plane() {}
    plane(vec3 normal, float d, material mat) : normal(normal), d(d), mat(mat) {}

    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const
    {
        // hit function for plane
        float denom = r.direction.dot(normal);
        if (denom > 1e-6)
        {
            vec3 p0l0 = normal * d - r.origin;
            float t = p0l0.dot(normal) / denom;
            if (t >= t_min && t <= t_max)
            {
                rec.t = t;
                rec.p = r.point_at_parameter(t);
                rec.normal = normal;
                rec.mat = mat;
                rec.income = r.direction.normalize();
                return true;
            }
        }
        return false;
    }
};

class sphere : public hitable
{
public:
    vec3 center;
    float radius;
    material mat;

    sphere() {}
    sphere(vec3 center, float radius, material mat) : center(center), radius(radius), mat(mat) {}

    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const
    {
        vec3 oc = r.origin - center;
        float a = r.direction.dot(r.direction);
        float b = oc.dot(r.direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - a * c;

        if (discriminant > 0)
        {
            float temp = (-b - sqrt(discriminant)) / a;
            if (temp < t_max && temp > t_min)
            {
                rec.t = temp;
                rec.p = r.point_at_parameter(rec.t);
                rec.normal = (rec.p - center) / radius;
                rec.mat = mat;
                rec.income = r.direction.normalize();
                return true;
            }

            temp = (-b + sqrt(discriminant)) / a;
            if (temp < t_max && temp > t_min)
            {
                rec.t = temp;
                rec.p = r.point_at_parameter(rec.t);
                rec.normal = (rec.p - center) / radius;
                rec.mat = mat;
                rec.income = r.direction.normalize();
                return true;
            }
        }

        return false;
    }
};

class triangle : public hitable
{
public:
    vec3 v0, v1, v2;
    material mat;

    triangle() {}
    triangle(vec3 v0, vec3 v1, vec3 v2, material mat) : v0(v0), v1(v1), v2(v2), mat(mat) {}

    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const
    {
        vec3 e1 = v1 - v0;
        vec3 e2 = v2 - v0;
        vec3 p = r.direction.cross(e2);
        float a = e1.dot(p);

        if (a > -1e-6 && a < 1e-6)
            return false;

        float f = 1 / a;
        vec3 s = r.origin - v0;
        float u = f * s.dot(p);

        if (u < 0 || u > 1)
            return false;

        vec3 q = s.cross(e1);
        float v = f * r.direction.dot(q);

        if (v < 0 || u + v > 1)
            return false;

        float t = f * e2.dot(q);

        if (t < t_max && t > t_min)
        {
            rec.t = t;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = e1.cross(e2).normalize();
            rec.mat = mat;
            rec.income = r.direction.normalize();
            return true;
        }

        return false;
    }
};

class hitable_list : public hitable
{
public:
    hitable **list;
    int list_size;

    hitable_list() {}
    hitable_list(hitable **list, int list_size) : list(list), list_size(list_size) {}

    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const
    {
        hit_record temp_rec;
        bool hit_anything = false;
        float closest_so_far = t_max;

        for (int i = 0; i < list_size; i++)
        {
            if (list[i]->hit(r, t_min, closest_so_far, temp_rec))
            {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }
};

#endif