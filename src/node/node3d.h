#ifndef NODE_3D_H
#define NODE_3D_H

#include "../core/math.h"

class Node3D {
public:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

    Node3D();
    ~Node3D();

    glm::mat4 get_model_matrix();
    glm::vec3 get_forward();

};

#endif
