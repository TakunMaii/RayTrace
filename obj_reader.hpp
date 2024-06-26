#ifndef OBJREADER_HPP

#define OBJREADER_HPP

#include "vec3.hpp"
#include "objects.hpp"

// read a .obj file and return a hitable_list which contains all the triangles in the file
hitable_list* read_obj(const char *filename, material triangle_mat, float scale = 1, vec3 bios = vec3(0,0,0), bool autosetBoucing = false)
{
    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        printf("file not found\n");
        exit(1);
    }
    printf("reading obj file %s\n", filename);

    hitable **list = new hitable *[10000];
    int num = 0;;
    
    vec3 *vs = new vec3[10000];
    int vnum = 0;

    vec3 *vns = new vec3[10000];
    int vnnum = 0;

    vec3 bb1 = vec3(0,0,0);
    vec3 bb2 = vec3(0,0,0);

    // read lines
    char line[1000];
    while (fgets(line, 1000, f) != NULL)
    {
        if (line[0] == 'v')
        {
            if(line[1] == ' ')
            {
                vec3 v;
                sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z);
                vec3 changed = v * scale + bios;
                vs[vnum++] = changed;
                if(vnum == 1)
                {
                    bb1 = bb2 = changed;
                }
                bb1.x = min(bb1.x, changed.x);
                bb2.x = max(bb2.x, changed.x);
                bb1.y = min(bb1.y, changed.y);
                bb2.y = max(bb2.y, changed.y);
                bb1.z = min(bb1.z, changed.z);
                bb2.z = max(bb2.z, changed.z);
            }
            else if(line[1] == 'n')
            {
                vec3 v;
                sscanf(line, "vn %f %f %f", &v.x, &v.y, &v.z);
                vns[vnnum++] = v;
            }
            // ignore vt
        }
        else if (line[0] == 'f')
        {
            //detect if there are 4 verticles
            int v1, v2, v3, v4, vn1, vn2, vn3, vn4;
            int vt1, vt2, vt3, vt4;
            
            if(sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3, &v4, &vt4, &vn4) == 12)
            {
                // quad
                list[num++] = new triangle(vs[v1 - 1], vs[v2 - 1], vs[v3 - 1], triangle_mat);
                list[num++] = new triangle(vs[v1 - 1], vs[v3 - 1], vs[v4 - 1], triangle_mat);
            }
            else
            {
                sscanf(line, "f %d//%d %d//%d %d//%d", &v1, &vn1, &v2, &vn2, &v3, &vn3);
                list[num++] = new triangle(vs[v1 - 1], vs[v2 - 1], vs[v3 - 1], triangle_mat);
            }

        }
    }


    printf("read %d triangles, Done!\n", num);
    if(autosetBoucing)
    return (new hitable_list(list, num))->setBoucingBox(bb1,bb2);
    else
    return (new hitable_list(list, num));
}

#endif