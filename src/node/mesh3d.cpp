#include "mesh3d.h"
#include "../core/viewport.h"

Mesh3D::Mesh3D(
    Mesh *mesh,
    Shader *shader
) {
    this->mesh = mesh;
    this->shader = shader;
    glGenBuffers(1, &this->uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, this->uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(Matrices), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Mesh3D::~Mesh3D() {
    glDeleteBuffers(1, &this->uboMatrices);
}

void Mesh3D::draw() {
    if (this->shader == nullptr || this->mesh == nullptr) {
        return;
    }
    this->shader->use();

    Matrices matrices = this->get_matrices();

    glBindBuffer(GL_UNIFORM_BUFFER, this->uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrices), &matrices);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    this->shader->bind_block(this->uboMatrices, 0, "Matrices");

    Viewport *vp = Viewport::get_main_viewport();
    if (vp != nullptr && vp->light != nullptr) {
        vp->light->shader_bind_data(this->shader, 1, "LightData");
    }

    this->mesh->draw();
}
