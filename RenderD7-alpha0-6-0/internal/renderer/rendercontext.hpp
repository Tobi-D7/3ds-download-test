#pragma once
#include <citro3d.h>
#include <3ds.h>
namespace d7gfx {
    
    class RenderContext {
    public:
       
        enum class ScreenTarget {
            Top,     ///< Top screen
            Bottom   ///< Bottom/touch screen
        };

        enum class Stereo3dSide {
            Left, ///< Left side
            Right ///< Right side
        };

        enum class Mode {
            Flat,   ///< 2D
            Spatial ///< 3D
        };

        
        RenderContext(
            int t_modelUniform,
            bool t_3dEnabled,
            d7gfx::RenderContext::Mode t_mode,
            d7gfx::RenderContext::Stereo3dSide t_side,
            d7gfx::RenderContext::ScreenTarget t_target,
            C3D_Mtx& t_model,
            C3D_LightEnv& t_lightEnv,
            C3D_Light& t_light,
            C3D_LightLut& t_lightLut
        );

        int getModelUniform();

        bool is3dEnabled();

        d7gfx::RenderContext::Mode getMode();

        d7gfx::RenderContext::Stereo3dSide getSide();

        d7gfx::RenderContext::ScreenTarget getScreenTarget();

        C3D_Mtx& getModelMatrix();

        C3D_LightEnv& getLightEnvironment();

        C3D_Light& getLight();

        C3D_LightLut& getLightLut();

        void enableTextures(bool t_enable);

    private:
        /* data */
        int m_modelUniformLocation;
        bool m_3dEnabled;
        d7gfx::RenderContext::Mode m_mode;
        d7gfx::RenderContext::Stereo3dSide m_side;
        d7gfx::RenderContext::ScreenTarget m_target;
        C3D_Mtx &m_model;
        C3D_LightEnv& m_lightEnv;
        C3D_Light& m_light;
        C3D_LightLut& m_lightLut;
    };
} /* d7gfx */
