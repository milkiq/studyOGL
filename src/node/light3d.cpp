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

void Light3DMesh::draw(Camera *camera, float aspect) {
    shader->use();
    shader->set_vec3("light_color", color);
    shader->set_mat4("view_matrix", camera->get_view_matrix());
    shader->set_mat4("model_matrix", get_model_matrix());
    shader->set_mat4("projection_matrix", camera->get_projection_matrix(aspect));
    mesh->draw();
}
