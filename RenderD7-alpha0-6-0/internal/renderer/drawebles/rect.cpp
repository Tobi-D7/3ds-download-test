#include "rect.hpp"

namespace d7gfx {
    Rect::Rect(int t_x, int t_y, int t_width, int t_height, d7gfx::Color t_color) :
        m_posX(t_x),
        m_posY(t_y),
        m_width(t_width),
        m_height(t_height),
        m_color(t_color) {
            updateVertices();
    }

    void Rect::setXPosition(int t_x) {
        m_posX = t_x;
        updateVertices();
    }

    int Rect::getXPosition() {
        return m_posX;
    }

    void Rect::setYPosition(int t_y) {
        m_posY = t_y;
        updateVertices();
    }

    int Rect::getYPosition() {
        return m_posY;
    }

    void Rect::setPosition(int t_x, int t_y) {
        m_posX = t_x;
        m_posY = t_y;
        updateVertices();
    }

    void Rect::setWidth(int t_width) {
        m_width = t_width;
        updateVertices();
    }

    int Rect::getWidth() {
        return m_width;
    }

    void Rect::setHeight(int t_height) {
        m_height = t_height;
        updateVertices();
    }

    int Rect::getHeight() {
        return m_height;
    }

    d7gfx::BoundingBox Rect::getBoundingBox() {
        return d7gfx::BoundingBox(m_posX, m_posY, m_width, m_height);
    }

    void Rect::setColor(d7gfx::Color t_color) {
        m_color = t_color;
        updateVertices();
    }

    d7gfx::Color Rect::getColor() {
        return m_color;
    }

    // private methods
    void Rect::updateVertices() {
        clearVertices();
        addVertex(m_posX, m_posY, m_color);
        addVertex(m_posX, m_posY + m_height, m_color);
        addVertex(m_posX + m_width, m_posY + m_height, m_color);
        addVertex(m_posX + m_width, m_posY, m_color);
    }
} /* d7gfx */