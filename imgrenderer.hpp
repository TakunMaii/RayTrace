#ifndef IMGRENDERER_HPP
#define IMGRENDERER_HPP

#include "vec3.hpp"
#include <string.h>

class img_renderer
{
public:
    int width, height;
    vec3 *pixels;
    vec3 (*fragment_func)(int x, int y) = nullptr;
    int number = 0;

    img_renderer(int width, int height, vec3 (*fragment)(int, int)) : width(width), height(height)
    {
        pixels = new vec3[width * height];
        memset(pixels, 0, width * height * sizeof(vec3));
        fragment_func = fragment;
    }

    ~img_renderer()
    {
        delete[] pixels;
    }

    void set_pixel(int x, int y, vec3 color)
    {
        pixels[y * width + x] = color;
    }

    vec3 get_pixel(int x, int y) const
    {
        return pixels[y * width + x];
    }

    void render()
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                set_pixel(x, y, fragment_func(x, height - 1 - y));
            }
            printf("rendering %.2f%%\r", (float)y / height * 100);
        }
    }

    void render_and_mix(const char * filename)
    {
        vec3 *temp = new vec3[width * height];
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                vec3 color = fragment_func(x, height - 1 - y);
                set_pixel(x, y, (get_pixel(x, y) * number + color)/(float(number) + 1.0));
            }
            printf("rendering %.2f%%\r", (float)y / height * 100);
        }
        number++;
        printf("mix one img, now number is %d\n", number);
        save(filename);
    }

    void save(const char *filename)
    {
        FILE *f = fopen(filename, "wb");
        fprintf(f, "P6\n%d %d\n255\n", width, height);
        for (int i = 0; i < width * height; i++)
        {
            if(pixels[i].x != pixels[i].x || pixels[i].y != pixels[i].y || pixels[i].z != pixels[i].z) {
                printf("nan detected\n");
                pixels[i] = vec3(0, 0, 0);
            }
            fputc((unsigned char)(255 * pixels[i].x), f);
            fputc((unsigned char)(255 * pixels[i].y), f);
            fputc((unsigned char)(255 * pixels[i].z), f);
        }
        fclose(f);
    }
};

#endif