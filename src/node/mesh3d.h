#ifndef MESH_3D_H
#define MESH_3D_H

#include "../core/shader.h"
#include "../core/mesh.h"
#include "node3d.h"
#include <glad/glad.h>

class Mesh3D : public Node3D {

private:
    Shader* shader = nullptr;
    Mesh* mesh = nullptr;

public:
    Mesh3D(
        Mesh *mesh,
        Shader *shader
    );
    ~Mesh3D();
    void draw();
};

#endif // MESH_3D_H
