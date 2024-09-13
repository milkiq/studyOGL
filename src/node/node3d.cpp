#include "node3d.h"

Node3D::Node3D() {}

Node3D::~Node3D() {}

glm::mat4 Node3D::get_model_matrix() {
    glm::mat4 model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, position);
    model_matrix = glm::rotate(model_matrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model_matrix = glm::rotate(model_matrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model_matrix = glm::rotate(model_matrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    model_matrix = glm::scale(model_matrix, scale);
    return model_matrix;
}
