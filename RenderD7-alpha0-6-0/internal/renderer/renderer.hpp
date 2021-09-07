#pragma once
#include <citro3d.h>
#include <3ds.h>

#include <cstring>
#include <functional>
#include <map>
#include <vector>


#include "rendercontext.hpp"
#include "renderTarget.hpp"
#include "color.hpp"
#include "camera.hpp"
#include "draweble.hpp"
#include "texture.hpp"
namespace d7gfx
{
    class Renderer
    {
        public:
        Renderer(bool _3d = false);
        ~Renderer();
        
        
        void Render(bool d_clear);
        void clear();

        void set3d(bool t_enabled);
        void useCulling(bool t_useCulling);
        bool isUsingCulling();
        void setWide(t_wide);
        
        void setClearColor(d7gfx::Color t_color);
        void setClearColor(d7gfx::Color t_color, d7gfx::RenderContext::ScreenTarget t_target);
        d7gfx::Color getClearColor(d7gfx::RenderContext::ScreenTarget t_target);
        void drawTop(d7gfx::Drawable& t_object, d7gfx::RenderContext::Mode t_mode = d7gfx::RenderContext::Mode::Flat, int t_layer = 0);
        void drawTop(d7gfx::Drawable& t_object, std::function<bool()> t_shadingFunction, d7gfx::RenderContext::Mode t_mode = d7gfx::RenderContext::Mode::Flat, int t_layer = 0);
        void drawBottom(d7gfx::Drawable& t_object, d7gfx::RenderContext::Mode t_mode = d7gfx::RenderContext::Mode::Flat, int t_layer = 0);
        void drawBottom(d7gfx::Drawable& t_object, std::function<bool()> t_shadingFunction, d7gfx::RenderContext::Mode t_mode = d7gfx::RenderContext::Mode::Flat, int t_layer = 0);
        static int getScreenWidth(d7gfx::RenderContext::ScreenTarget t_target);
        static int getScreenHeight();

        void setCamera(d7gfx::Camera t_camera, d7gfx::RenderContext::ScreenTarget t_target);
        d7gfx::Camera& getCamera(d7gfx::RenderContext::ScreenTarget t_target);
        void useFog(bool t_useFog, d7gfx::RenderContext::ScreenTarget t_target);
        bool getUseFog(d7gfx::RenderContext::ScreenTarget t_target);
        void setFogDensity(float t_density,d7gfx::RenderContext::ScreenTarget t_target);
        float getFogDensity(d7gfx::RenderContext::ScreenTarget t_target);
        private:
        /*Data*/

        
        void prepare();
        void prepareFog(d7gfx::RenderContext::ScreenTarget t_target);
        void prepareLights(d7gfx::RenderContext::ScreenTarget t_target);

        int m_projectionUniform, m_modelUniform, m_viewUniform;
        bool m_3dEnabled, m_useCulling;
        d7gfx::Color m_clearColorTop, m_clearColorBottom;
        d7gfx::RenderTarget *m_targetTopLeft, *m_targetTopRight, *m_targetBottom;
        d7gfx::Camera &m_cameraTop, &m_cameraBottom;

        std::map<int, std::vector<std::pair<d7gfx::Drawable*, std::function<bool()>>>, std::less<int>> m_drawStackTop2d, m_drawStackTop3d, m_drawStackBottom2d, m_drawStackBottom3d;
        // shader
        DVLB_s* m_dvlb;
        shaderProgram_s m_shader;

        // attribute information
        C3D_AttrInfo m_attributeInfo;

        // matrices
        C3D_Mtx m_projection, m_model, m_view;

        //lioght
        C3D_LightEnv m_lightEnvTop, m_lightEnvBottom;
        C3D_Light m_lightTop, m_lightBottom;
        C3D_LightLut m_lutPhongTop, m_lutPhongBottom;

        // fog
        bool m_useFogTop, m_useFogBottom;
        float m_fogDensityTop, m_fogDensityBottom;
        C3D_FogLut m_fogLutTop, m_fogLutBottom;
        //Skybox
        
        //Widemode
        bool m_wide = false;
        
    };
}