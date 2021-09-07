#include "renderer.hpp"
#include "_3d_shbin.h"
#include <citro2d.h>

d7gfx::Camera _1, _2;


namespace d7gfx
{
    Renderer::Renderer(bool _3d) :
            m_3dEnabled(_3d),
            m_useCulling(true),
            m_clearColorTop(d7gfx::Color(0, 0, 0)),
            m_clearColorBottom(d7gfx::Color(0, 0, 0)),
            m_cameraTop(_1),
            m_cameraBottom(_2),
            m_useFogTop(false),
            m_useFogBottom(false),
            m_fogDensityTop(0.05),
            m_fogDensityBottom(0.05)
    {
        C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
        m_targetTopLeft  = new d7gfx::RenderTarget(400, 240);
        m_targetTopRight = new d7gfx::RenderTarget(400, 240);
        m_targetBottom   = new d7gfx::RenderTarget(320, 240);

        u32 flags = GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO);

        C3D_RenderTargetSetOutput(m_targetTopLeft->getRenderTarget(),  GFX_TOP,    GFX_LEFT,  flags);
        C3D_RenderTargetSetOutput(m_targetTopRight->getRenderTarget(), GFX_TOP,    GFX_RIGHT, flags);
        C3D_RenderTargetSetOutput(m_targetBottom->getRenderTarget(),   GFX_BOTTOM, GFX_LEFT,  flags);

        if (_3d)
        {
            set3d(_3d);
        }
        
        //shader
         m_dvlb = DVLB_ParseFile((u32*) _3d_shbin, _3d_shbin_size);
        shaderProgramInit(&m_shader);
        shaderProgramSetVsh(&m_shader, &m_dvlb->DVLE[0]);

        // get location of uniforms used in the vertex shader.
        m_projectionUniform = shaderInstanceGetUniformLocation(m_shader.vertexShader, "projection");
        m_modelUniform = shaderInstanceGetUniformLocation(m_shader.vertexShader, "model");
        m_viewUniform = shaderInstanceGetUniformLocation(m_shader.vertexShader, "view");

        AttrInfo_Init(&m_attributeInfo);
        AttrInfo_AddLoader(&m_attributeInfo, 0, GPU_FLOAT, 3); // v0=position
        AttrInfo_AddLoader(&m_attributeInfo, 1, GPU_FLOAT, 2); // v1=texcoord
        AttrInfo_AddLoader(&m_attributeInfo, 2, GPU_FLOAT, 3); // v2=normal
        //light
        C3D_FVec lightVec;

        C3D_LightEnvInit(&m_lightEnvTop);
        LightLut_Phong(&m_lutPhongTop, 30);
        C3D_LightEnvLut(&m_lightEnvTop, GPU_LUT_D0, GPU_LUTINPUT_LN, false, &m_lutPhongTop);
        lightVec = FVec4_New(0.0f, 0.0f, -0.5f, 1.0f);
        C3D_LightInit(&m_lightTop, &m_lightEnvTop);
        C3D_LightColor(&m_lightTop, 1.0, 1.0, 1.0);
        C3D_LightPosition(&m_lightTop, &lightVec);

        C3D_LightEnvInit(&m_lightEnvBottom);
        LightLut_Phong(&m_lutPhongBottom, 30);
        C3D_LightEnvLut(&m_lightEnvBottom, GPU_LUT_D0, GPU_LUTINPUT_LN, false, &m_lutPhongBottom);
        lightVec = FVec4_New(0.0f, 0.0f, -0.5f, 1.0f);
        C3D_LightInit(&m_lightBottom, &m_lightEnvBottom);
        C3D_LightColor(&m_lightBottom, 1.0, 1.0, 1.0);
        C3D_LightPosition(&m_lightBottom, &lightVec);

        clear();
    }
    Renderer::~Renderer()
    {
        C2D_Fini();
        shaderProgramFree(&m_shader);
        DVLB_Free(m_dvlb);
        delete m_targetTopLeft;
        delete m_targetTopRight;
        delete m_targetBottom;
    }
    void Renderer::Render(bool d_clear)
    {
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        if (d_clear)
        {
            m_targetTopLeft->clear();
            m_targetTopRight->clear();
            m_targetBottom->clear();
            clear();
        }
        
        if(m_drawStackTop3d.size() > 0 || m_drawStackBottom3d.size() > 0) {
            prepare();
            C3D_DepthTest(true, GPU_GEQUAL, GPU_WRITE_ALL);
            Mtx_PerspStereoTilt(&m_projection, C3D_AngleFromDegrees(40.0f), C3D_AspectRatioBot, 0.01f, 1000.0f, 0, 2.0f, false);

            C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, m_projectionUniform, &m_projection);
            C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, m_viewUniform, &m_cameraBottom.getViewMatrix());

            if(m_drawStackBottom3d.size() > 0) {
                C3D_FrameDrawOn(m_targetBottom->getRenderTarget());
                prepareLights(d7gfx::RenderContext::ScreenTarget::Bottom);
                if (m_useFogBottom) prepareFog(d7gfx::RenderContext::ScreenTarget::Bottom);

                for(const auto &entry : m_drawStackBottom3d) { // for every layer
                    for(const auto &drawable : entry.second) { // draw every object
                        if (drawable.second()) {
                            drawable.first->draw(d7gfx::RenderContext(
                                m_modelUniform, // modelUniform
                                m_3dEnabled, // 3dEnabled
                                d7gfx::RenderContext::Mode::Spatial,        // mode
                                d7gfx::RenderContext::Stereo3dSide::Left,   // side
                                d7gfx::RenderContext::ScreenTarget::Bottom, // target
                                m_model,  // model
                                m_lightEnvBottom, // lightEnv
                                m_lightBottom,    // light
                                m_lutPhongBottom  // lutPhong
                            ));
                        }
                    }
                }

                m_drawStackBottom3d.clear();
            }

            if (m_drawStackTop3d.size() > 0) {
                C3D_FrameDrawOn(m_targetTopLeft->getRenderTarget());
                prepareLights(d7gfx::RenderContext::ScreenTarget::Top);
                if (m_useFogTop) prepareFog(d7gfx::RenderContext::ScreenTarget::Top);

                // tilt stereo perspective
                if (m_3dEnabled) {
                    Mtx_PerspStereoTilt(&m_projection, C3D_AngleFromDegrees(40.0f), C3D_AspectRatioTop, 0.01f, 1000.0f, -(osGet3DSliderState() / 3.0f), 2.0f, false);
                } else {
                    Mtx_PerspStereoTilt(&m_projection, C3D_AngleFromDegrees(40.0f), C3D_AspectRatioTop, 0.01f, 1000.0f, 0, 2.0f, false);
                }

                C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, m_projectionUniform, &m_projection);
                C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, m_viewUniform, &m_cameraTop.getViewMatrix());

                for (const auto &entry : m_drawStackTop3d) { // for every layer
                    for (const auto &drawable : entry.second) { // draw every object
                        if (drawable.second()) {
                            drawable.first->draw(d7gfx::RenderContext(
                                m_modelUniform, // modelUniform
                                m_3dEnabled, // 3dEnabled
                                d7gfx::RenderContext::Mode::Spatial,      // mode
                                d7gfx::RenderContext::Stereo3dSide::Left, // side
                                d7gfx::RenderContext::ScreenTarget::Top,  // target
                                m_model,  // model
                                m_lightEnvTop, // lightEnv
                                m_lightTop,    // light
                                m_lutPhongTop  // lutPhong
                            ));
                        }
                    }
                }

                if (m_3dEnabled && osGet3DSliderState() > 0.0f) {
                    C3D_FrameDrawOn(m_targetTopRight->getRenderTarget());

                    // tilt stereo perspective
                    if (m_3dEnabled) {
                        Mtx_PerspStereoTilt(&m_projection, C3D_AngleFromDegrees(40.0f), C3D_AspectRatioTop, 0.01f, 1000.0f, osGet3DSliderState() / 3.0f, 2.0f, false);
                    } else {
                        Mtx_PerspStereoTilt(&m_projection, C3D_AngleFromDegrees(40.0f), C3D_AspectRatioTop, 0.01f, 1000.0f, 0, 2.0f, false);
                    }

                    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, m_projectionUniform, &m_projection);

                    for (const auto &entry : m_drawStackTop3d) { // for every layer
                        for (const auto &drawable : entry.second) { // draw every object
                            if (drawable.second()) {
                                drawable.first->draw(d7gfx::RenderContext(
                                    m_modelUniform, // modelUniform
                                    m_3dEnabled, // 3dEnabled
                                    d7gfx::RenderContext::Mode::Spatial,       // mode
                                    d7gfx::RenderContext::Stereo3dSide::Right, // side
                                    d7gfx::RenderContext::ScreenTarget::Top,   // target
                                    m_model,  // model
                                    m_lightEnvTop, // lightEnv
                                    m_lightTop,    // light
                                    m_lutPhongTop  // lutPhong
                                ));
                            }
                        }
                    }
                }

                m_drawStackTop3d.clear();
            }
        }

        // draw 2d
        if (m_drawStackTop2d.size() > 0 || m_drawStackBottom2d.size() > 0) {
            C2D_Prepare();
            C3D_DepthTest(true, GPU_ALWAYS, GPU_WRITE_ALL);

            if(m_drawStackBottom2d.size() > 0) {
                C2D_SceneBegin(m_targetBottom->getRenderTarget());

                for(const auto &entry : m_drawStackBottom2d) { // for every layer
                    for(const auto &drawable : entry.second) { // draw every object
                        if (drawable.second()) {
                            drawable.first->draw(d7gfx::RenderContext(
                                m_modelUniform, // modelUniform
                                m_3dEnabled, // 3dEnabled
                                d7gfx::RenderContext::Mode::Flat,           // mode
                                d7gfx::RenderContext::Stereo3dSide::Left,   // side
                                d7gfx::RenderContext::ScreenTarget::Bottom, // target
                                m_model,  // model
                                m_lightEnvBottom, // lightEnv
                                m_lightBottom,    // light
                                m_lutPhongBottom  // lutPhong
                            ));
                        }
                    }
                }

                m_drawStackBottom2d.clear();
            }

            if(m_drawStackTop2d.size() > 0) {
                C2D_SceneBegin(m_targetTopLeft->getRenderTarget());

                for(const auto &entry : m_drawStackTop2d) { // for every layer
                    for(const auto &drawable : entry.second) { // draw every object
                        if (drawable.second()) {
                            drawable.first->draw(d7gfx::RenderContext(
                                m_modelUniform, // modelUniform
                                m_3dEnabled, // 3dEnabled
                                d7gfx::RenderContext::Mode::Flat,         // mode
                                d7gfx::RenderContext::Stereo3dSide::Left, // side
                                d7gfx::RenderContext::ScreenTarget::Top,  // target
                                m_model,  // model
                                m_lightEnvTop, // lightEnv
                                m_lightTop,    // light
                                m_lutPhongTop  // lutPhong
                            ));
                        }
                    }
                }

                if(m_3dEnabled && osGet3DSliderState() > 0.0f) {
                    C2D_SceneBegin(m_targetTopRight->getRenderTarget());

                    for(const auto &entry : m_drawStackTop2d) { // for every layer
                        for(const auto &drawable : entry.second) { // draw every object
                            if (drawable.second()) {
                                drawable.first->draw(d7gfx::RenderContext(
                                    m_modelUniform, // modelUniform
                                    m_3dEnabled, // 3dEnabled
                                    d7gfx::RenderContext::Mode::Flat,          // mode
                                    d7gfx::RenderContext::Stereo3dSide::Right, // side
                                    d7gfx::RenderContext::ScreenTarget::Top,   // target
                                    m_model,  // model
                                    m_lightEnvTop, // lightEnv
                                    m_lightTop,    // light
                                    m_lutPhongTop  // lutPhong
                                ));
                            }
                        }
                    }
                }

                m_drawStackTop2d.clear();
            }
        }

        C3D_FrameEnd(0);
    }

    void Renderer::clear() {
        C2D_TargetClear(m_targetTopLeft->getRenderTarget(), m_clearColorTop.getRgba8());
        C2D_TargetClear(m_targetTopRight->getRenderTarget(), m_clearColorTop.getRgba8());
        C2D_TargetClear(m_targetBottom->getRenderTarget(), m_clearColorBottom.getRgba8());
    }

    void Renderer::set3d(bool t_enabled) {
        gfxSet3D(t_enabled);
        m_3dEnabled = t_enabled;
    }

    void Renderer::useCulling(bool t_useCulling) {
        m_useCulling = t_useCulling;
    }

    bool Renderer::isUsingCulling() {
        return m_useCulling;
    }

    void Renderer::setClearColor(d7gfx::Color t_color) {
        m_clearColorTop = t_color;
        m_clearColorBottom = t_color;
    }

    void Renderer::setClearColor(d7gfx::Color t_color, d7gfx::RenderContext::ScreenTarget t_target) {
        t_target == d7gfx::RenderContext::ScreenTarget::Top ?
                    m_clearColorTop = t_color :
                    m_clearColorBottom = t_color;
    }

    d7gfx::Color Renderer::getClearColor(d7gfx::RenderContext::ScreenTarget t_target) {
        return t_target == d7gfx::RenderContext::ScreenTarget::Top ? m_clearColorTop : m_clearColorBottom;
    }

    void Renderer::drawTop(d7gfx::Drawable& t_object, d7gfx::RenderContext::Mode t_mode, int t_layer) {
        if (t_mode == d7gfx::RenderContext::Mode::Flat) {
            if(m_drawStackTop2d.count(t_layer) > 0) {
                m_drawStackTop2d[t_layer].insert(m_drawStackTop2d[t_layer].end(), std::make_pair(&t_object, [](){return true;}));
            } else {
                std::vector<std::pair<d7gfx::Drawable*, std::function<bool()>>> newStack;
                newStack.push_back(std::make_pair(&t_object, [](){return true;}));
                m_drawStackTop2d.insert(std::make_pair(t_layer, newStack));
            }
        } else {
            if(m_drawStackTop3d.count(t_layer) > 0) {
                m_drawStackTop3d[t_layer].insert(m_drawStackTop3d[t_layer].end(), std::make_pair(&t_object, [](){return true;}));
            } else {
                std::vector<std::pair<d7gfx::Drawable*, std::function<bool()>>> newStack;
                newStack.push_back(std::make_pair(&t_object, [](){return true;}));
                m_drawStackTop3d.insert(std::make_pair(t_layer, newStack));
            }
        }
    }

    void Renderer::drawTop(d7gfx::Drawable& t_object, std::function<bool()> t_shadingFunction, d7gfx::RenderContext::Mode t_mode, int t_layer) {
        if (t_mode == d7gfx::RenderContext::Mode::Flat) {
            if(m_drawStackTop2d.count(t_layer) > 0) {
                m_drawStackTop2d[t_layer].insert(m_drawStackTop2d[t_layer].end(), std::make_pair(&t_object, t_shadingFunction));
            } else {
                std::vector<std::pair<d7gfx::Drawable*, std::function<bool()>>> newStack;
                newStack.push_back(std::make_pair(&t_object, t_shadingFunction));
                m_drawStackTop2d.insert(std::make_pair(t_layer, newStack));
            }
        } else {
            if(m_drawStackTop3d.count(t_layer) > 0) {
                m_drawStackTop3d[t_layer].insert(m_drawStackTop3d[t_layer].end(), std::make_pair(&t_object, t_shadingFunction));
            } else {
                std::vector<std::pair<d7gfx::Drawable*, std::function<bool()>>> newStack;
                newStack.push_back(std::make_pair(&t_object, t_shadingFunction));
                m_drawStackTop3d.insert(std::make_pair(t_layer, newStack));
            }
        }
    }

    void Renderer::drawBottom(d7gfx::Drawable& t_object, d7gfx::RenderContext::Mode t_mode, int t_layer) {
        if (t_mode == d7gfx::RenderContext::Mode::Flat) {
            if(m_drawStackBottom2d.count(t_layer) > 0) {
                m_drawStackBottom2d[t_layer].insert(m_drawStackBottom2d[t_layer].end(), std::make_pair(&t_object, [](){return true;}));
            } else {
                std::vector<std::pair<d7gfx::Drawable*, std::function<bool()>>> newStack;
                newStack.push_back(std::make_pair(&t_object, [](){return true;}));
                m_drawStackBottom2d.insert(std::make_pair(t_layer, newStack));
            }
        } else {
            if(m_drawStackBottom3d.count(t_layer) > 0) {
                m_drawStackBottom3d[t_layer].insert(m_drawStackBottom3d[t_layer].end(), std::make_pair(&t_object, [](){return true;}));
            } else {
                std::vector<std::pair<d7gfx::Drawable*, std::function<bool()>>> newStack;
                newStack.push_back(std::make_pair(&t_object, [](){return true;}));
                m_drawStackBottom3d.insert(std::make_pair(t_layer, newStack));
            }
        }
    }

    void Renderer::drawBottom(d7gfx::Drawable& t_object, std::function<bool()> t_shadingFunction, d7gfx::RenderContext::Mode t_mode, int t_layer) {
        if (t_mode == d7gfx::RenderContext::Mode::Flat) {
            if(m_drawStackBottom2d.count(t_layer) > 0) {
                m_drawStackBottom2d[t_layer].insert(m_drawStackBottom2d[t_layer].end(), std::make_pair(&t_object, t_shadingFunction));
            } else {
                std::vector<std::pair<d7gfx::Drawable*, std::function<bool()>>> newStack;
                newStack.push_back(std::make_pair(&t_object, t_shadingFunction));
                m_drawStackBottom2d.insert(std::make_pair(t_layer, newStack));
            }
        } else {
            if(m_drawStackBottom3d.count(t_layer) > 0) {
                m_drawStackBottom3d[t_layer].insert(m_drawStackBottom3d[t_layer].end(), std::make_pair(&t_object, t_shadingFunction));
            } else {
                std::vector<std::pair<d7gfx::Drawable*, std::function<bool()>>> newStack;
                newStack.push_back(std::make_pair(&t_object, t_shadingFunction));
                m_drawStackBottom3d.insert(std::make_pair(t_layer, newStack));
            }
        }
    }


    int Renderer::getScreenWidth(d7gfx::RenderContext::ScreenTarget t_target) {
        return t_target == d7gfx::RenderContext::ScreenTarget::Top ? 400 : 320;
    }

    int Renderer::getScreenHeight() {
        return 240;
    }

    void Renderer::setCamera(d7gfx::Camera t_camera, d7gfx::RenderContext::ScreenTarget t_target) {
        switch (t_target) {
            case d7gfx::RenderContext::ScreenTarget::Top:
                m_cameraTop = t_camera;
                break;
            default:
                m_cameraBottom = t_camera;
        }
    }

    d7gfx::Camera& Renderer::getCamera(d7gfx::RenderContext::ScreenTarget t_target) {
        return (t_target == d7gfx::RenderContext::ScreenTarget::Top ? m_cameraTop : m_cameraBottom);
    }

    void Renderer::useFog(bool t_useFog, d7gfx::RenderContext::ScreenTarget t_target) {
        if (t_target == d7gfx::RenderContext::ScreenTarget::Top) {
            m_useFogTop = t_useFog;
        } else {
            m_useFogBottom = t_useFog;
        }
    }

    bool Renderer::getUseFog(d7gfx::RenderContext::ScreenTarget t_target) {
        return (t_target == d7gfx::RenderContext::ScreenTarget::Top ? m_useFogTop : m_useFogBottom);
    }

    void Renderer::setFogDensity(float t_density, d7gfx::RenderContext::ScreenTarget t_target) {
        if (t_target == d7gfx::RenderContext::ScreenTarget::Top) {
            m_fogDensityTop = t_density;
        } else {
            m_fogDensityBottom = t_density;
        }
    }

    float Renderer::getFogDensity(d7gfx::RenderContext::ScreenTarget t_target) {
        return (t_target == d7gfx::RenderContext::ScreenTarget::Top ? m_fogDensityTop : m_fogDensityBottom);
    }

    // private methods
    void Renderer::prepare() {
        C3D_BindProgram(&m_shader);

        // initialize and configure attributes
        C3D_SetAttrInfo(&m_attributeInfo);

        // Configure the first fragment shading substage to blend the fragment primary color
        // with the fragment secondary color.
        // See https://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml for more insight
        C3D_TexEnv* env = C3D_GetTexEnv(0);
        C3D_TexEnvInit(env);
        C3D_TexEnvSrc(env, C3D_Both, GPU_FRAGMENT_PRIMARY_COLOR, GPU_FRAGMENT_SECONDARY_COLOR, GPU_PRIMARY_COLOR);
        C3D_TexEnvFunc(env, C3D_Both, GPU_ADD);

        C3D_TexEnvInit(C3D_GetTexEnv(2));
        C3D_TexEnvInit(C3D_GetTexEnv(3));
        C3D_TexEnvInit(C3D_GetTexEnv(4));
        C3D_TexEnvInit(C3D_GetTexEnv(5));

        C3D_CullFace(m_useCulling ? GPU_CULL_BACK_CCW : GPU_CULL_NONE);
    }

    void Renderer::prepareFog(d7gfx::RenderContext::ScreenTarget t_target) {
        switch (t_target) {
            case d7gfx::RenderContext::ScreenTarget::Bottom:
                FogLut_Exp(&m_fogLutBottom, m_fogDensityBottom, 1.5f, 0.01f, 20.0f);
                C3D_FogGasMode(GPU_FOG, GPU_PLAIN_DENSITY, false);
                C3D_FogColor(m_clearColorTop.getRgb8());
                C3D_FogLutBind(&m_fogLutBottom);
                break;
            default:
                FogLut_Exp(&m_fogLutTop, m_fogDensityTop, 1.5f, 0.01f, 20.0f);
                C3D_FogGasMode(GPU_FOG, GPU_PLAIN_DENSITY, false);
                C3D_FogColor(m_clearColorTop.getRgb8());
                C3D_FogLutBind(&m_fogLutTop);
        }
    }

    void Renderer::prepareLights(d7gfx::RenderContext::ScreenTarget t_target) {
        switch (t_target) {
            case d7gfx::RenderContext::ScreenTarget::Bottom:
                C3D_LightEnvBind(&m_lightEnvBottom);
                break;
            default:
                C3D_LightEnvBind(&m_lightEnvTop);
        }
    }
    void Renderer::setWide(bool t_wide)
    {
        this->m_wide = t_wide;
        
    }
    
}