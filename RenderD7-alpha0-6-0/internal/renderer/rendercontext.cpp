#include "rendercontext.hpp"

namespace d7gfx {
    RenderContext::RenderContext(
        int t_modelUniform,
        bool t_3dEnabled,
        d7gfx::RenderContext::Mode t_mode,
        d7gfx::RenderContext::Stereo3dSide t_side,
        d7gfx::RenderContext::ScreenTarget t_target,
        C3D_Mtx& t_model,
        C3D_LightEnv& t_lightEnv,
        C3D_Light& t_light,
        C3D_LightLut& t_lightLut
    ) :
    m_modelUniformLocation(t_modelUniform),
    m_3dEnabled(t_3dEnabled),
    m_mode(t_mode),
    m_side(t_side),
    m_target(t_target),
    m_model(t_model),
    m_lightEnv(t_lightEnv),
    m_light(t_light),
    m_lightLut(t_lightLut) { /* do nothing */ }

    int RenderContext::getModelUniform() {
        return m_modelUniformLocation;
    }

    bool RenderContext::is3dEnabled() {
        return m_3dEnabled;
    }

    d7gfx::RenderContext::Mode RenderContext::getMode() {
        return m_mode;
    }

    d7gfx::RenderContext::Stereo3dSide RenderContext::getSide() {
        return m_side;
    }

    d7gfx::RenderContext::ScreenTarget RenderContext::getScreenTarget() {
        return m_target;
    }

    C3D_Mtx& RenderContext::getModelMatrix() {
        return m_model;
    }

    C3D_LightEnv& RenderContext::getLightEnvironment() {
        return m_lightEnv;
    }

    C3D_Light& RenderContext::getLight() {
        return m_light;
    }

    C3D_LightLut& RenderContext::getLightLut() {
        return m_lightLut;
    }

    void RenderContext::enableTextures(bool t_enable) {
        C3D_TexEnv* env;

        switch (t_enable) {
            case true:
                // ambient/diffuse
                env = C3D_GetTexEnv(0);
                C3D_TexEnvInit(env);
                C3D_TexEnvSrc(env, C3D_Both, GPU_TEXTURE0, GPU_FRAGMENT_PRIMARY_COLOR, GPU_PRIMARY_COLOR);
                C3D_TexEnvFunc(env, C3D_Both, GPU_MODULATE);

                // specular
                env = C3D_GetTexEnv(1);
                C3D_TexEnvInit(env);
                C3D_TexEnvSrc(env, C3D_Both, GPU_PREVIOUS, GPU_FRAGMENT_SECONDARY_COLOR, GPU_PRIMARY_COLOR);
                C3D_TexEnvFunc(env, C3D_Both, GPU_ADD);
                break;
            default:
                env = C3D_GetTexEnv(0);
                C3D_TexEnvInit(env);
                C3D_TexEnvSrc(env, C3D_Both, GPU_FRAGMENT_PRIMARY_COLOR, GPU_FRAGMENT_SECONDARY_COLOR, GPU_PRIMARY_COLOR);
                C3D_TexEnvFunc(env, C3D_Both, GPU_ADD);
        }
    }
} /* d7gfx */