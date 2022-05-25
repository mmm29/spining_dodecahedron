#include <cassert>
#include <unordered_map>

#include "sfml_renderer.h"

using render::SFMLRenderer;


void SFMLRenderer::SetRenderTarget(sf::RenderTarget *render_target) {
    render_target_ = render_target;
}

void SFMLRenderer::BindVertexBuffer(const Vertex *buffer, uint32_t count, PrimitiveTopology topology) {
    buffer_ = buffer;
    vertices_count_ = count;
    primitive_topology_ = topology;
}

static sf::Color ColorToSfmlColor(const Color &color) {
    return sf::Color(color.r, color.g, color.b, color.a);
}

void SFMLRenderer::Draw(uint32_t vertex_count, uint32_t first_vertex) {
    assert(render_target_ != nullptr);
    assert(buffer_ != nullptr);
    assert(first_vertex + vertex_count <= vertices_count_);

    sf::PrimitiveType sfml_primitive_type;

    if (primitive_topology_ == kLines)
        sfml_primitive_type = sf::PrimitiveType::Lines;
    else if (primitive_topology_ == kTriangles)
        sfml_primitive_type = sf::PrimitiveType::Triangles;
    else
        assert(false);

    sf::VertexArray vertices(sfml_primitive_type, vertex_count);

    for (size_t vertex_idx = first_vertex; vertex_idx < vertex_count; vertex_idx++) {
        const Vertex &vertex = buffer_[vertex_idx];
        vertices[vertex_idx] = sf::Vertex{sf::Vector2f(vertex.position[0], vertex.position[1]),
                                          ColorToSfmlColor(vertex.color)};
    }

    render_target_->draw(vertices);
}