#include "light3d.h"

Light3D::Light3D() {
    color = glm::vec3(1.0f, 1.0f, 1.0f);
}

Light3D::~Light3D() {
}

Light3DMesh::Light3DMesh(
    Mesh *mesh,
    Shader *shader
) {
    this->mesh = mesh;
    this->shader = shader;
}

Light3DMesh::~Light3DMesh() {
}

void Light3DMesh::draw() {
    if (shader == nullptr || mesh == nullptr) {
        return;
    }
    shader->use();
    shader->set_vec3("light_color", color);
    shader->set_mat4("model_matrix", get_model_matrix());
    mesh->draw();
}
