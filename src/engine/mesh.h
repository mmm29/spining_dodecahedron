#pragma once

#include <vector>

#include "math/vector.h"
#include "math/color.h"
#include "math/matrix.h"

class Mesh {
public:
    struct Vertex {
        Vector3 position;
        Color color;
    };

    struct Face {
        std::vector<uint32_t> indices;
    };

    void SetVertices(std::vector<Vertex> &&vertices);

    void SetFaces(std::vector<Face> &&faces);

    void Transform(const Matrix4& transform);

protected:
    std::vector<Vertex> vertices_;
    std::vector<Face> faces_;
};