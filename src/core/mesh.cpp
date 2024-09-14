#include "mesh.h"
#include <iostream>

// vertice data contains position, normal, color and texture coordinates
Mesh::Mesh(std::vector<Vertex> &vertex_data, std::vector<unsigned int> &index_data) {
    this->index_count = index_data.size();

    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(Vertex), &vertex_data[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_data.size() * sizeof(unsigned int), &index_data[0], GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    // texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
    glEnableVertexAttribArray(2);
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

bool Mesh::is_valid() {
    return VAO != 0 && VBO != 0 && EBO != 0;
}

void Mesh::draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
}
