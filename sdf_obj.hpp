#ifndef SDFOBJ_HPP
#define SDFOBJ_HPP

#include "vec3.hpp"
#include "objects.hpp"
#include <functional>
using namespace std;

using SDF = function<float(vec3)>;

class sdf_hitable : public hitable
{
public:
    float delta = 0.001; // 计算梯度用的
    int max_step = 100;
    float tolerance = 0.005;

    /*
    signed distance function, the min distance to the surface.
    */
    SDF sdf;

    material mat;

    sdf_hitable(SDF sdf, material mat)
    {
        this->sdf = sdf;
        this->mat = mat;
    }

    vec3 normal_at(vec3 p)const
    {
        float ps = sdf(p);
        return vec3(sdf(p + vec3(delta, 0, 0)) - ps, sdf(p + vec3(0, delta, 0)) - ps, sdf(p + vec3(0, 0, delta)) - ps).normalize();
    }

    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const
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
                // printf("hit sdf obj in p %.2f %.2f %.2f",point.x,point.y,point.z);
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

SDF create_line_sdf(vec3 dir, vec3 o)
{
    return [&](vec3 p)
    {
        vec3 diff = o - p;
        float l = diff.length();
        float h = diff.dot(dir);
        return sqrt(l*l - h*h);
    };
}

SDF and_sdf(SDF s1, SDF s2)
{
    return [&](vec3 p)
    {
        return max(s1(p), s2(p));
    };
}

SDF or_sdf(SDF s1, SDF s2)
{
    return [&](vec3 p)
    {
        return min(s1(p), s2(p));
    };
}

SDF diff_sdf(SDF s1, SDF s2)
{
    return [&](vec3 p)
    {
        return max(s1(p), -s2(p));
    };
}
#endif