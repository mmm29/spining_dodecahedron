#pragma once

#include <cstdint>

#include "../math/vector.h"
#include "../math/color.h"

namespace render {

    enum PrimitiveTopology {
        kLines,
        kTriangles
    };

    struct Vertex {
        Vector2 position;
        Color color;
    };

    class Renderer {
    public:
        virtual ~Renderer() = default;

        virtual void BindVertexBuffer(const Vertex *buffer, uint32_t count, PrimitiveTopology topology) = 0;

        virtual void Draw(uint32_t vertex_count, uint32_t first_vertex) = 0;
    };

}