#ifndef BOX_MODEL_H
#define BOX_MODEL_H

#include "model.h"

float box_vertices[] = {
    -0.25, 0.25, 0.25,
    0.25, 0.25, -0.25,
    0.25, 0.25, 0.25,
    -0.25, 0.25, -0.25,
    -0.25, -0.25, 0.25,
    0.25, -0.25, -0.25,
    0.25, -0.25, 0.25,
    -0.25, -0.25, -0.25,
    0.25, 0.25, 0.25,
    -0.25, 0.25, -0.25,
    0.25, 0.25, -0.25,
    -0.25, 0.25, 0.25,
    0.25, -0.25, 0.25,
    -0.25, -0.25, -0.25,
    0.25, -0.25, -0.25,
    -0.25, -0.25, 0.25,
    0.25, 0.25, 0.25,
    -0.25, -0.25, 0.25,
    -0.25, 0.25, 0.25,
    0.25, -0.25, 0.25,
    0.25, 0.25, -0.25,
    -0.25, -0.25, -0.25,
    -0.25, 0.25, -0.25,
    0.25, -0.25, -0.25
};

float box_normals[] = {
    0, 0, 1,
    0, 0, -1,
    0, 0, 1,
    0, 0, -1,
    0, 0, 1,
    0, 0, -1,
    0, 0, 1,
    0, 0, -1,
    1, 0, 0,
    -1, 0, 0,
    1, 0, 0,
    -1, 0, 0,
    1, 0, 0,
    -1, 0, 0,
    1, 0, 0,
    -1, 0, 0,
    0, 1, 0,
    0, -1, 0,
    0, 1, 0,
    0, -1, 0,
    0, 1, 0,
    0, -1, 0,
    0, 1, 0,
    0, -1, 0
};

float box_tex_coords[] = {
    0, 1,
    0, 1,
    1, 1,
    1, 1,
    0, 0,
    0, 0,
    1, 0,
    1, 0,

    0, 1,
    0, 1,
    1, 1,
    1, 1,
    0, 0,
    0, 0,
    1, 0,
    1, 0,

    1, 0,
    1, 0,
    0, 0,
    0, 0,
    1, 1,
    1, 1,
    0, 1,
    0, 1
};

unsigned int box_indices[] = {
    0, 4, 2,
    2, 4, 6,
    1, 5, 3,
    3, 5, 7,
    8, 12, 10,
    10, 12, 14,
    9, 13, 11,
    11, 13, 15,
    16, 20, 18,
    18, 20, 22,
    17, 21, 19,
    19, 21, 23
};


class BoxModel : public Model
{
public:
    BoxModel() {
        unsigned int vertex_count = sizeof(box_vertices) / sizeof(*box_vertices) / 3;

        std::vector<Mesh::Vertex> vertex_data;
        vertex_data.reserve(vertex_count);

        for (unsigned int i = 0; i < vertex_count; i++) {
            Mesh::Vertex vertex;
            
            vertex.position = glm::vec3(
                box_vertices[i * 3],
                box_vertices[i * 3 + 1],
                box_vertices[i * 3 + 2]
            );
            
            vertex.normal = glm::vec3(
                box_normals[i * 3],
                box_normals[i * 3 + 1],
                box_normals[i * 3 + 2]
            );

            vertex.tex_coords = glm::vec2(
                box_tex_coords[i * 2],
                box_tex_coords[i * 2 + 1]
            );
            
            vertex_data.push_back(vertex);
        }

        std::vector<unsigned int> index_data(box_indices, box_indices + sizeof(box_indices) / sizeof(*box_indices));

        set_vertices(vertex_data);
        set_indices(index_data);
    }
    ~BoxModel() {}
};

#endif // BOX_MODEL_H
