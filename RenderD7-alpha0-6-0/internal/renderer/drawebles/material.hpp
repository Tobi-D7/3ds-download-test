#pragma once
#include <citro3d.h>
#include "color.hpp"

namespace d7gfx {
    
    class Material {
    public:
        
        Material();
        Material(int kar, int kag, int kab, int kdr, int kdg, int kdb, int ksr, int ksg, int ksb);
        Material(C3D_Material t_material);
        Material(const d7gfx::Material& t_source);
        virtual ~Material();
        void setMaterial(C3D_Material t_material);
        C3D_Material* getMaterial() const;
        void setAmbient(d7gfx::Color t_color);
        void setAmbient(int t_r, int t_g, int t_b);
        d7gfx::Color getAmbient();
        void setDiffuse(d7gfx::Color t_color);
        void setDiffuse(int t_r, int t_g, int t_b);
        d7gfx::Color getDiffuse();
        void setSpecular0(d7gfx::Color t_color);
        void setSpecular0(int t_r, int t_g, int t_b);
        d7gfx::Color getSpecular0();
        void setSpecular1(d7gfx::Color t_color);
        void setSpecular1(int t_r, int t_g, int t_b);
        d7gfx::Color getSpecular1();
        void setEmission(d7gfx::Color t_color);
        void setEmission(int t_r, int t_g, int t_b);
        d7gfx::Color getEmission();
        d7gfx::Material& operator=(const d7gfx::Material& rhs);

    private:
        void updateMaterial();

        /* data */
        d7gfx::Color m_ambient, m_diffuse, m_specular0, m_specular1, m_emission;
        C3D_Material* m_material;
    };
} /* d7gfx */
