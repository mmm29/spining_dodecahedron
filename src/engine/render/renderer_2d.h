#pragma once

#include "renderer.h"

namespace render {

    class Renderer2D {
    public:
        explicit Renderer2D(Renderer *renderer);

        void DrawLine(const Vector2 &p1, const Vector2 &p2, const Color &color);

        void DrawTriangle(const Vector2 &p1, const Vector2 &p2, const Vector2 &p3, const Color &color);

    private:
        Renderer *renderer_;
    };

}