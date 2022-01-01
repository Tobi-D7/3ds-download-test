#include "plane.hpp"

namespace d7gfx {
    Plane::Plane(float t_length, float t_width) {
        m_length = t_length;
        m_width = t_width;
        update();
    }

    void Plane::setLength(float t_length) {
        m_length = t_length;
    }

    float Plane::getLength() {
        return m_length;
    }

    void Plane::setWidth(float t_width) {
        m_width = t_width;
    }

    float Plane::getWidth() {
        return m_width;
    }

    // private methods
    void Plane::update() {
        clearVertices();

        // Face
        addPolygon(d7gfx::Mesh::Polygon(
            { {-(m_width / 2), -1, (m_length / 2)}, {0.0f, 0.0f}, {0.0f, 0.0f, +1.0f} },
            { {(m_width / 2), -1, (m_length / 2)}, {1.0f, 0.0f}, {0.0f, 0.0f, +1.0f} },
            { {(m_width / 2), 1, (m_length / 2)}, {1.0f, 1.0f}, {0.0f, 0.0f, +1.0f} }
        ));

        updateVBO();
    }
}
