#ifndef MESH_H
#define MESH_H

#include "math.h"
#include <vector>
#include <iostream>
#include <glad/glad.h>

class Mesh
{
private:
    unsigned int VAO, VBO, EBO;
    unsigned int index_count;

public:
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 tex_coords;
    };

    Mesh(std::vector<Vertex> &vertex_data, std::vector<unsigned int> &index_data);
    ~Mesh();

    bool is_valid();
    void draw();
};


#endif // MESH_H
