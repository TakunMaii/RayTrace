#ifndef SDFOBJ_HPP
#define SDFOBJ_HPP

#include "vec3.hpp"
#include "objects.hpp"

using SDF=functional<int()>;

class sdf_hitable : public hitable
{
public:
    float delta = 0.001; // 计算梯度用的
    int max_step = 1000;
    float tolerance = 0.005;

    /*
    signed distance function, the min distance to the surface.
    */
    virtual float sdf(vec3 p) = 0;

    material mat;

    vec3 normal_at(vec3 p)
    {
        float ps = sdf(p);
        return vec3(sdf(p + vec3(delta, 0, 0)) - ps, sdf(p + vec3(0, delta, 0)) - ps, sdf(p + vec3(0, 0, delta)) - ps).normalize();
    }

    bool hit(const ray &r, float t_min, float t_max, hit_record &rec)
    {
        float t = t_min;
        int counter = 0;
        while (t < t_max && counter < max_step)
        {
            vec3 point = r.point_at_parameter(t);
            float sdf_t = sdf(point);
            if (abs(sdf_t) < tolerance)
            {
                // hit
                rec = hit_record();
                rec.income = r.direction.normalize();
                rec.mat = mat;
                rec.normal = normal_at(point);
                rec.p = point;
                rec.t = t;
                return true;
            }
            // continue marching
            t += sdf_t;
        }
        return false;
    }
};

SDF create_shpere_sdf(vec3 center, float radius)
{
    return [&](vec3 p)
    {
        return (center - p).length() - radius;
    };
}


#endif