#include "renderer_2d.h"

using render::Renderer2D;

Renderer2D::Renderer2D(Renderer *renderer) : renderer_(renderer) {
}

void Renderer2D::DrawLine(const Vector2 &p1, const Vector2 &p2, const Color &color) {
    Vertex vertices[2];

    vertices[0].position = p1;
    vertices[0].color = color;

    vertices[1].position = p2;
    vertices[1].color = color;

    renderer_->BindVertexBuffer(vertices, 2, PrimitiveTopology::kLines);

    renderer_->Draw(2, 0);
}