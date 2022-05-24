#include "mesh.h"

void Mesh::SetVertices(std::vector<Vertex> &&vertices) {
    vertices_ = std::move(vertices);
}

void Mesh::SetFaces(std::vector<Face> &&faces) {
    faces_ = std::move(faces);
}

void Mesh::Transform(const Matrix4 &transform) {
    for (Vertex &vertex : vertices_)
        vertex.position = (transform * vertex.position.AsVec4()).AsVec3();
}

const std::vector<Mesh::Vertex> &Mesh::GetVertices() const {
    return vertices_;
}

const std::vector<Mesh::Face> &Mesh::GetFaces() const {
    return faces_;
}