#ifndef LIGHT_3D_H
#define LIGHT_3D_H

#include <glad/glad.h>
#include "../core/shader.h"
#include "node3d.h"

class Light3D : public Node3D {
protected:
    GLuint uboLightData = -1;

public:
    struct LightData {
        glm::vec3 position;
        float padding1[1];
        glm::vec3 color;
        float padding2[1];
    };

    glm::vec3 color;

    Light3D();
    ~Light3D();

    LightData get_data();
    void shader_bind_data(const Shader *shader, const unsigned int block_index, const char *block_name) const;
};

#endif
