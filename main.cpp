#include "imgrenderer.hpp"
#include "obj_reader.hpp"
#include "objects.hpp"
#include <iostream>
#include <stdlib.h>
#include <string>

const int width = 400;
const int height = 300;
const vec3 campos = vec3(0, 1, 2);
const vec3 camray = vec3(0, -0.6, -2).normalize();
const vec3 cam_horizontal_axis = vec3(4, 0, 0);
const vec3 cam_vertical_axis = cam_horizontal_axis.cross(camray).normalize() * 3;
const float cam_to_screen = 2.2;
const float t_min = 0.01, t_max = 500;
const int max_bounce_time = 5;
const int montecarlo_count = 1;
const vec3 skyray = vec3(1, -2, -0.4).normalize();
const float sky_strength = 2;
const float blur = 0.14;

hitable_list *world;

vec3 get_sky_color(vec3 r)
{
    r = r.normalize();
    vec3 white = vec3(0.8, 0.8, 0.8);
    vec3 blue = vec3(0.8, 0.8, 0.9);
    float mix_factor = (r.y + 1) / 2;
    return white * (1 - mix_factor) + blue * mix_factor;
}

void setup_world()
{
    hitable **list = new hitable *[10];
    int num = 0;

    list[num++] = new sphere(vec3(-0.8, 0.23, 0.95), 0.23, material(vec3(0.7, 0.7, 0.7), 0.95, 0.8));
    list[num++] = new sphere(vec3(0.3, 0.3, 1.1), 0.3, material(vec3(0.5, 0.7, 0.5), 0.9, 0.92).set_refrection(0.7, 0.7));
    list[num++] = new sphere(vec3(0.84, 0.6, 0), 0.6, material(vec3(1, 0.3, 0.5), 0.5, 0.5));
    list[num++] = new sphere(vec3(0.2, 1.4, -2.2), 1.4, material(vec3(0.4, 0.3, 0.5), 0.95, 0.98).set_refrection(0.8, 1));
    list[num++] = new sphere(vec3(-0.2, 0.12, 0.6), 0.12, material(vec3(1, 1, 1), 0.5, 0.5).set_emission(vec3(1, 1, 1), 3.5));
    list[num++] = read_obj("model.obj", material(vec3(0.4, 0.8, 0.7), 0.5, 0.5), 0.02, vec3(-0.6, 0, 0));
    list[num++] = new plane(vec3(0, -1, 0), 0, material(vec3(0.4, 0.4, 0.4), 0.3, 0.5));
    world = new hitable_list(list, num);
}

vec3 get_screen_pos(float u, float v)
{
    return campos + camray * cam_to_screen + cam_horizontal_axis * (u - 0.5) + cam_vertical_axis * (v - 0.5);
}

void ray_trace(ray r, vec3 lightCol, vec3 &pixelCol, int depth)
{
    if (depth > max_bounce_time)
        return;

    hit_record hitrec;
    bool does_hit = world->hit(r, t_min, t_max, hitrec);
    if (does_hit)
    {
        float choose = random_float();
        vec3 out;
        if (choose > hitrec.mat.reflection_strength)
        {
            // 漫反射
            out = random_positive_to(hitrec.normal);
        }
        else
        {
            choose = random_float();
            if (choose > hitrec.mat.refraction_strength)
            {
                // 镜面反射
                out = hitrec.normal * -2 * hitrec.normal.dot(hitrec.income) + hitrec.income;
                out = out.normalize();
            }
            else
            {
                // 折射
                out = (-hitrec.normal * hitrec.mat.refraction_index + hitrec.income).normalize();
            }
        }
        ray outray = ray(hitrec.p, out);
        lightCol = lightCol * hitrec.mat.color;
        float factor = - r.direction.dot(hitrec.normal);
        factor = (factor + 1) /2;
        pixelCol = pixelCol + lightCol * hitrec.mat.emission_color * hitrec.mat.emission_strength * factor;
        pixelCol.limit();
        ray_trace(outray, lightCol, pixelCol, depth + 1);
    }
    else
    {
        // not hit anything, which means shot to the sky
        vec3 skycol = get_sky_color(r.direction);
        if (depth == 0)
        {
            pixelCol = skycol;
            return;
        }
        float strfactor = -skyray.dot(r.direction);
        strfactor = (strfactor + 1) / 2;
        pixelCol = pixelCol + lightCol * skycol * sky_strength * strfactor;
        pixelCol.limit();
    }
}

vec3 fragshader(int x, int y)
{
    float u = float(x) / width;
    float v = float(y) / height;

    vec3 screen_pos = get_screen_pos(u, v);

    vec3 color = vec3(0, 0, 0);
    for (int i = 0; i < montecarlo_count; i++)
    {
        vec3 curr_campos = campos + (cam_horizontal_axis.normalize() * random_float() * blur) + (cam_vertical_axis.normalize() * random_float() * blur);
        ray eyeray = ray(curr_campos, (screen_pos - curr_campos).normalize());
        vec3 pixelCol = vec3(0, 0, 0);
        ray_trace(eyeray, vec3(1, 1, 1), pixelCol, 0);
        color = color + pixelCol;
    }

    return color / montecarlo_count;
}

int main()
{
    // initial_rand(341);
    setup_world();
    img_renderer img = img_renderer(width, height, fragshader);
    // img.render();
    // img.save("output.ppm");
    while (true)
    {
        img.render_and_mix("light2.ppm");
    }
    return 0;
}