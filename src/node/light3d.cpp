#include "light3d.h"

Light3D::Light3D() {
    color = glm::vec3(1.0f, 1.0f, 1.0f);
    glGenBuffers(1, &uboLightData);
    glBindBuffer(GL_UNIFORM_BUFFER, uboLightData);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(LightData), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Light3D::~Light3D() {
    glDeleteBuffers(1, &uboLightData);
}

Light3D::LightData Light3D::get_data() {
    LightData data;
    data.color = color;
    data.position = position;
    return data;
}

void Light3D::shader_bind_data(const Shader *shader, const unsigned int block_index, const char *block_name) const {
    LightData data;
    data.color = color;
    data.position = position;
    glBindBuffer(GL_UNIFORM_BUFFER, uboLightData);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightData), &data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    shader->bind_block(uboLightData, block_index, block_name);
}
