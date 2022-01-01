
#pragma once
#include "mesh.hpp"

namespace d7gfx {
    
    class Plane: public d7gfx::Mesh {
    public:
        Plane(float t_length, float t_width);
        void setLength(float t_length);
        float getLength();
        void setWidth(float t_width);
        float getWidth();

    private:
        void update();
        float m_length, m_width;
    };
}
