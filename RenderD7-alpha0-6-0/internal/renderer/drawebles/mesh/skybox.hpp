#pragma once
#include "mesh.hpp"

namespace d7gfx {
    
    class SkyBox: public d7gfx::Mesh {
    public:
        SkyBox(float t_size);
        
        void setSize(float t_size);
        float getSize();
        void SetSkyBoxTex(std::string sheet);
    private:
        void update();
        float m_size;
    };
} /* d7gfx */

