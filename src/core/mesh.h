#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

class Mesh
{
private:
    unsigned int VAO, VBO, EBO;
    unsigned int verticesSize;
    unsigned int indicesSize;

public:
    Mesh(float *vertices, unsigned int verticesSize, unsigned int *indices, unsigned int indicesSize);
    ~Mesh();

    void draw();
};


#endif // MESH_H
