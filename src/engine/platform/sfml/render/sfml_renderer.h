#pragma once

#include <SFML/Graphics.hpp>

#include "../../../render/renderer.h"

namespace render {

    class SFMLRenderer : public Renderer {
    public:
        void SetRenderTarget(sf::RenderTarget *render_target);

        void BindVertexBuffer(const Vertex *buffer, uint32_t count, PrimitiveTopology topology) override;

        void Draw(uint32_t vertex_count, uint32_t first_vertex) override;

    private:
        sf::RenderTarget *render_target_ = nullptr;

    private:
        const Vertex *buffer_ = nullptr;
        uint32_t vertices_count_ = 0;
        PrimitiveTopology primitive_topology_;
    };

}