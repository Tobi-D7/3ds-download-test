#pragma once
#include "mesh.hpp"

namespace d7gfx {
    
    class Cube: public d7gfx::Mesh {
    public:
        Cube(float t_length, float t_width, float t_height);
        void setLength(float t_length);
        float getLength();
        void setWidth(float t_width);
        float getWidth();

        void setHeight(float t_height);
        float getHeight();

    private:
        void update();
        float m_length, m_width, m_height;
    };
} /* d7gfx */

