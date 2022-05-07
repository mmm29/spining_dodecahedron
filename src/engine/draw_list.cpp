#include "draw_list.h"

using draw::DrawList;

void DrawList::Clear() {
    primitives.clear();
}

void DrawList::AddPrimitive(Primitive &&primitive) {
    primitives.push_back(std::move(primitive));
}

void DrawList::AddPrimitive(PrimitiveType type, std::vector<Vertex> &&vertices) {
    AddPrimitive(Primitive{type, std::move(vertices)});
}

void DrawList::AddLine(const Vector2 &p1, const Vector2 &p2, Color color, float thickness) {
    // TODO: use thickness

    AddPrimitive(kPrimitiveLine, {
            Vertex{p1, color},
            Vertex{p2, color}
    });
}

void DrawList::AddTriangle(const Vector2 &p1, const Vector2 &p2, const Vector2 &p3, Color color, float thickness) {
    AddLine(p1, p2, color, thickness);
    AddLine(p2, p3, color, thickness);
    AddLine(p3, p1, color, thickness);
}

void DrawList::AddFilledTriangle(const Vector2 &p1, const Vector2 &p2, const Vector2 &p3, Color color) {
    AddPrimitive(kPrimitiveTriangle, {
            Vertex{p1, color},
            Vertex{p2, color},
            Vertex{p3, color}
    });
}