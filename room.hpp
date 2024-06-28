#include "vec3.hpp"
#include "objects.hpp"
#include "obj_reader.hpp"

hitable_list *generator_room()
{
    hitable **list = new hitable *[10000];
    int num = 0;
    float length = 10;

    // build room with triangles
    vec3 v1 = vec3(-length, -length, -length);
    vec3 v2 = vec3(length, -length, -length);
    vec3 v3 = vec3(length, length, -length);
    vec3 v4 = vec3(-length, length, -length);
    vec3 v5 = vec3(-length, -length, length);
    vec3 v6 = vec3(length, -length, length);
    vec3 v7 = vec3(length, length, length);
    vec3 v8 = vec3(-length, length, length);

    // matierals
    material red = material(vec3(1, 0.3, 0.2), 0, 0.8);
    material green = material(vec3(0.2, 1, 0.3), 0, 0.8);
    material blue = material(vec3(0.3, 0.2, 1), 0, 0.8);
    material white = material(vec3(1, 1, 1), 0, 0.8);
    material yellow = material(vec3(1, 1, 0.2), 0, 0.8);
    material purple = material(vec3(1, 0.3, 1), 0, 0.8);
    material light = material(vec3(1, 1, 1), 0.9, 1).set_emission(vec3(1, 1, 1), 5);

    // ceiling
    list[num++] = new triangle(v3, v2, v1, white);
    list[num++] = new triangle(v1, v4, v3, white);
    // floor
    list[num++] = new triangle(v5, v6, v7, red);
    list[num++] = new triangle(v7, v8, v5, red);
    // back
    list[num++] = new triangle(v1, v2, v6, green);
    list[num++] = new triangle(v6, v5, v1, green);
    // front
    list[num++] = new triangle(v3, v4, v8, blue);
    list[num++] = new triangle(v8, v7, v3, blue);
    // left
    list[num++] = new triangle(v1, v5, v8, yellow);
    list[num++] = new triangle(v8, v4, v1, yellow);
    // right
    list[num++] = new triangle(v3, v7, v6, purple);
    list[num++] = new triangle(v6, v2, v3, purple);

    // lamp on the ceiling
    vec3 lamp1 = vec3(-5, length - 0.1, 5);
    vec3 lamp2 = vec3(5, length - 0.1, 5);
    vec3 lamp3 = vec3(5, length - 0.1, -5);
    vec3 lamp4 = vec3(-5, length - 0.1, -5);
    list[num++] = new triangle(lamp1, lamp2, lamp3, light);
    list[num++] = new triangle(lamp3, lamp4, lamp1, light);

    //model
    hitable_list *model = read_obj("model.obj", material(vec3(0.7, 0.7, 0.7), 0.7, 0.7),0.03,vec3(-1.5,-length/3+1,0.3),true);
    hitable_list *model2 = read_obj("objs/bishop_01.obj", material(vec3(0.7, 0.7, 0.7), 0.7, 0.7).set_refrection(0.99),0.04,vec3(1.3,1.3,-1.1),true);
    list[num++] = model;
    list[num++] = model2;
    return new hitable_list(list, num);
}