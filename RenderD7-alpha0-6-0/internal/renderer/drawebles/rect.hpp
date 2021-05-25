#pragma once
#include "shape.hpp"
#include "boundingBox.hpp"

namespace d7gfx {
    /**
     * @brief The Rect shape
     * @todo Implement support for gradients
     * @todo Implement rotation
     */
    class Rect: public d7gfx::Shape {
    public:
        /**
         * @brief Creates the Rect
         * @param t_x      The x position of the Rect
         * @param t_y      The y position of the Rect
         * @param t_width  The width of the Rect
         * @param t_height The height of the Rect
         * @param t_color  The fill color of the Rect
         */
        Rect(int t_x, int t_y, int t_width, int t_height, d7gfx::Color t_color);

        /**
         * @brief Sets the x position of the Rect
         * @param t_x The x position
         */
        void setXPosition(int t_x);

        /**
         * @brief Returns the x position of the Rect
         * @return The x position
         */
        int getXPosition();

        /**
         * @brief Sets the y position of the Rect
         * @param t_y The y position
         */
        void setYPosition(int t_y);

        /**
         * @brief Returns the y position of the Rect
         * @return The y position
         */
        int getYPosition();

        /**
         * @brief Sets the x and y position of the Rect
         * @param t_x The x position
         * @param t_y The y position
         */
        void setPosition(int t_x, int t_y);

        /**
         * @brief Sets the width of the Rect
         * @param t_width The width
         */
        void setWidth(int t_width);

        /**
         * @brief Returns the width of the Rect
         * @return The width
         */
        int getWidth();

        /**
         * @brief Sets the height of the Rect
         * @param t_height The height
         */
        void setHeight(int t_height);

        /**
         * @brief Returns the height of the Rect
         * @return The height
         */
        int getHeight();

        /**
         * @brief Returns the bounding box of the Rect
         * @return The bounding box
         */
        d7gfx::BoundingBox getBoundingBox();

        /**
         * @brief Sets the fill color of the Rect
         * @param t_color The color
         */
        void setColor(d7gfx::Color t_color);

        /**
         * @brief Returns the fill color of the Rect
         * @return The color
         */
        d7gfx::Color getColor();

    private:
        void updateVertices();

        /* data */
        int m_posX, m_posY, m_width, m_height;
        d7gfx::Color m_color;
    };
} /* d7gfx */