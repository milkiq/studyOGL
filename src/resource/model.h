#ifndef MODEL_H
#define MODEL_H

#include "../core/mesh.h"

class Model
{
private:
    std::vector<Mesh::Vertex> vertices;
    std::vector<unsigned int> indices;

public:
    Model() {};
    ~Model() {};

    void set_vertices(std::vector<Mesh::Vertex> &vertices) {
        this->vertices = vertices;
    }

    void set_indices(std::vector<unsigned int> &indices) {
        this->indices = indices;
    }

    std::vector<Mesh::Vertex> *get_vertices() {
        return &this->vertices;
    }

    std::vector<unsigned int> *get_indices() {
        return &this->indices;
    }
};

#endif // MODEL_H
