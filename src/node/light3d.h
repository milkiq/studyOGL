#ifndef LIGHT_3D_H
#define LIGHT_3D_H

#include "../core/shader.h"
#include "../core/mesh.h"
#include "camera.h"
#include "node3d.h"

class Light3D : public Node3D {
public:
    glm::vec3 color;

    Light3D();
    ~Light3D();
};

class Light3DMesh : public Light3D {
private:
    Mesh *mesh;
    Shader *shader;

public:
    Light3DMesh(
        Mesh *mesh,
        Shader *shader
    );
    ~Light3DMesh();

    void draw(Camera *camera, float aspect);
};

#endif
