#pragma once

#include <list>
#include <vector>

#include "vector.h"
#include "color.h"

namespace draw {

    enum PrimitiveType {
        kPrimitiveLine,
        kPrimitiveTriangle
    };

    struct Vertex {
        Vector2 position;
        Color color;
    };

    struct Primitive {
        PrimitiveType type;
        std::vector<Vertex> vertices;
    };

    struct DrawList {
        std::list<Primitive> primitives;

        void Clear();

        // Primitives
        void AddPrimitive(Primitive &&primitive);

        void AddPrimitive(PrimitiveType type, std::vector<Vertex> &&vertices);

        void AddLine(const Vector2 &p1, const Vector2 &p2, Color color, float thickness = 1.f);

        void AddTriangle(const Vector2 &p1, const Vector2 &p2, const Vector2 &p3, Color color, float thickness = 1.f);

        void AddFilledTriangle(const Vector2 &p1, const Vector2 &p2, const Vector2 &p3, Color color);
    };

}