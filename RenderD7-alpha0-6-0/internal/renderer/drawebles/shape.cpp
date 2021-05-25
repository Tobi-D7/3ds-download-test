#include <citro2d.h>
#include "earcut.hpp"
#include "shape.hpp"

namespace d7gfx {
    Shape::Shape() :
            m_changed(false) { /* do nothing */ }

    void Shape::addVertex(int t_x, int t_y, d7gfx::Color t_color) {
        m_vertices.push_back(std::make_pair((d7gfx::Vector2f) { (float) t_x, (float) t_y }, t_color));
        m_changed = true;
    }

    void Shape::addVertex(d7gfx::Vector2f t_vertex, d7gfx::Color t_color) {
        m_vertices.push_back(std::make_pair((d7gfx::Vector2f) { t_vertex.u, t_vertex.v }, t_color));
        m_changed = true;
    }

    void Shape::clearVertices() {
        m_vertices.clear();
        m_changed = true;
    }

    void Shape::draw(d7gfx::RenderContext t_context) {
        if (m_changed) {
            m_changed = false;

            std::vector<std::vector<std::array<double, 2>>> polygon;
            std::vector<std::array<double, 2>> polyline;

            for (const auto &vertex : m_vertices) {
                polyline.push_back({ vertex.first.u, vertex.first.v });
            }

            polygon.push_back(polyline);
            m_indices = mapbox::earcut<uint16_t>(polygon);
        }

        for (unsigned int i = 0; i < m_indices.size(); i += 3) {
            C2D_DrawTriangle(
                        m_vertices[m_indices[i]].first.u,
                        m_vertices[m_indices[i]].first.v,
                        m_vertices[m_indices[i]].second.getRgba8(),

                        m_vertices[m_indices[i + 1]].first.u,
                        m_vertices[m_indices[i + 1]].first.v,
                        m_vertices[m_indices[i + 1]].second.getRgba8(),

                        m_vertices[m_indices[i + 2]].first.u,
                        m_vertices[m_indices[i + 2]].first.v,
                        m_vertices[m_indices[i + 2]].second.getRgba8(),

                        0.5f
                    );
        }
    }
} /* d7gfx */