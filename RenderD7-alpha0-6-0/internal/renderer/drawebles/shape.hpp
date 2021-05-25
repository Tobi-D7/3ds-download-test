#pragma once
#include <vector>
#include "draweble.hpp"
#include "color.hpp"
#include "vertex.hpp"

namespace d7gfx {
    /**
     * @brief The base class for all shapes
     * @todo Add support for stereoscopic 3D
     * @todo Implement support for textures
     * @todo Add support for holes
     */
    class Shape: public d7gfx::Drawable {
    public:
        /**
         * @brief Initializes the shape
         */
        Shape();

        /**
         * @brief Adds a vertex given it's position and color
         * @param t_x     The x position
         * @param t_y     The y position
         * @param t_color The color
         */
        void addVertex(int t_x, int t_y, d7gfx::Color t_color);

        /**
         * @brief Adds a vertex given it's position and color
         * @param t_vertex The position, represented as a 2-dimensional vector
         * @param t_color  The color
         */
        void addVertex(d7gfx::Vector2f t_vertex, d7gfx::Color t_color);

        /**
         * @brief Removes all vertices
         */
        void clearVertices();

        /**
         * @brief Draws the shape
         * @param t_context the RenderContext
         */
        void draw(d7gfx::RenderContext t_context);

    private:
        /* data */
        bool m_changed;
        std::vector<std::pair<d7gfx::Vector2f, d7gfx::Color>> m_vertices;
        std::vector<uint16_t> m_indices;
    };
} /* d7gfx */