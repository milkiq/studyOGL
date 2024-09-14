#include "mesh3d.h"

Mesh3D::Mesh3D(
    Mesh *mesh,
    Shader *shader
) {
    this->mesh = mesh;
    this->shader = shader;
}

Mesh3D::~Mesh3D() {}

void Mesh3D::draw() {
    if (this->shader == nullptr || this->mesh == nullptr) {
        return;
    }
    this->shader->use();
    glm::mat4 model_matrix = this->get_model_matrix();
    glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(model_matrix)));
    this->shader->set_mat4("model_matrix", model_matrix);
    this->shader->set_mat3("normal_matrix", normal_matrix);
    this->mesh->draw();
}
