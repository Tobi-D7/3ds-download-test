#include "skybox.hpp"


//SkyBox
C3D_Tex sktex;
C3D_TexCube sk_cube;

bool genskybox(C3D_Tex *tex, C3D_TexCube *tex_cube, std::string pathtosheet)
{
    FILE *file = fopen(pathtosheet.c_str(), "rb");
    if(!file) return false;
    Tex3DS_Texture texsk = Tex3DS_TextureImportStdio(file, &sktex, &sk_cube, true);
    if (!texsk) return false;
    Tex3DS_TextureFree(texsk);
    fclose(file);
    return true;
}


namespace d7gfx {
    SkyBox::SkyBox(float t_size) {
        m_size = t_size;
        update();
    }

    

    void SkyBox::setSize(float t_size) {
        m_size = t_size;
    }

    float SkyBox::getSize() {
        return m_size;
    }

    // private methods
    void SkyBox::update() {
        clearVertices();

        // first face (PZ)
        addPolygon(d7gfx::Mesh::Polygon(
            { {-(m_size / 2), -(m_size / 2), (m_size / 2)}, {0.0f, 0.0f}, {0.0f, 0.0f, +1.0f} },
            { {(m_size / 2), -(m_size / 2), (m_size / 2)}, {1.0f, 0.0f}, {0.0f, 0.0f, +1.0f} },
            { {(m_size / 2), (m_size / 2), (m_size / 2)}, {1.0f, 1.0f}, {0.0f, 0.0f, +1.0f} }
        ));

        addPolygon(d7gfx::Mesh::Polygon(
            { {(m_size / 2), (m_size / 2), (m_size / 2)}, {1.0f, 1.0f}, {0.0f, 0.0f, +1.0f} },
            { {-(m_size / 2), (m_size / 2), (m_size / 2)}, {0.0f, 1.0f}, {0.0f, 0.0f, +1.0f} },
            { {-(m_size / 2), -(m_size / 2), (m_size / 2)}, {0.0f, 0.0f}, {0.0f, 0.0f, +1.0f} }
        ));

        // second face (MZ)
        addPolygon(d7gfx::Mesh::Polygon(
            { {-(m_size / 2), -(m_size / 2), -(m_size / 2)}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f} },
            { {-(m_size / 2), (m_size / 2), -(m_size / 2)}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f} },
            { {(m_size / 2), (m_size / 2), -(m_size / 2)}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f} }
        ));

        addPolygon(d7gfx::Mesh::Polygon(
            { {(m_size / 2), (m_size / 2), -(m_size / 2)}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f} },
            { {(m_size / 2), -(m_size / 2), -(m_size / 2)}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f} },
            { {-(m_size / 2), -(m_size / 2), -(m_size / 2)}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f} }
        ));

        // third face (PX)
        addPolygon(d7gfx::Mesh::Polygon(
            { {(m_size / 2), -(m_size / 2), -(m_size / 2)}, {0.0f, 0.0f}, {+1.0f, 0.0f, 0.0f} },
            { {(m_size / 2), (m_size / 2), -(m_size / 2)}, {1.0f, 0.0f}, {+1.0f, 0.0f, 0.0f} },
            { {(m_size / 2), (m_size / 2), (m_size / 2)}, {1.0f, 1.0f}, {+1.0f, 0.0f, 0.0f} }
        ));

        addPolygon(d7gfx::Mesh::Polygon(
            { {(m_size / 2), (m_size / 2), (m_size / 2)}, {1.0f, 1.0f}, {+1.0f, 0.0f, 0.0f} },
            { {(m_size / 2), -(m_size / 2), (m_size / 2)}, {0.0f, 1.0f}, {+1.0f, 0.0f, 0.0f} },
            { {(m_size / 2), -(m_size / 2), -(m_size / 2)}, {0.0f, 0.0f}, {+1.0f, 0.0f, 0.0f} }
        ));

        // fourth face (MX)
        addPolygon(d7gfx::Mesh::Polygon(
            { {-(m_size / 2), -(m_size / 2), -(m_size / 2)}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f} },
            { {-(m_size / 2), -(m_size / 2), (m_size / 2)}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f} },
            { {-(m_size / 2), (m_size / 2), (m_size / 2)}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f} }
        ));

        addPolygon(d7gfx::Mesh::Polygon(
            { {-(m_size / 2), (m_size / 2), (m_size / 2)}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f} },
            { {-(m_size / 2), (m_size / 2), -(m_size / 2)}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f} },
            { {-(m_size / 2), -(m_size / 2), -(m_size / 2)}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f} }
        ));

        // fifth face (PY)
        addPolygon(d7gfx::Mesh::Polygon(
            { {-(m_size / 2), (m_size / 2), -(m_size / 2)}, {0.0f, 0.0f}, {0.0f, +1.0f, 0.0f} },
            { {-(m_size / 2), (m_size / 2), (m_size / 2)}, {1.0f, 0.0f}, {0.0f, +1.0f, 0.0f} },
            { {(m_size / 2), (m_size / 2), (m_size / 2)}, {1.0f, 1.0f}, {0.0f, +1.0f, 0.0f} }
        ));

        addPolygon(d7gfx::Mesh::Polygon(
            { {(m_size / 2), (m_size / 2), (m_size / 2)}, {1.0f, 1.0f}, {0.0f, +1.0f, 0.0f} },
            { {(m_size / 2), (m_size / 2), -(m_size / 2)}, {0.0f, 1.0f}, {0.0f, +1.0f, 0.0f} },
            { {-(m_size / 2), (m_size / 2), -(m_size / 2)}, {0.0f, 0.0f}, {0.0f, +1.0f, 0.0f} }
        ));

        // sixth face (MY)
        addPolygon(d7gfx::Mesh::Polygon(
            { {-(m_size / 2), -(m_size / 2), -(m_size / 2)}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} },
            { {(m_size / 2), -(m_size / 2), -(m_size / 2)}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f} },
            { {(m_size / 2), -(m_size / 2), (m_size / 2)}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f} }
        ));

        addPolygon(d7gfx::Mesh::Polygon(
            { {(m_size / 2), -(m_size / 2), (m_size / 2)}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f} },
            { {-(m_size / 2), -(m_size / 2), (m_size / 2)}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f} },
            { {-(m_size / 2), -(m_size / 2), -(m_size / 2)}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} }
        ));

        updateVBO();
    }
    void SkyBox::SetSkyBoxTex(std::string sheet)
    {
        if(!genskybox(&sktex, &sk_cube, sheet)) svcBreak(USERBREAK_PANIC);
        C3D_TexSetFilter(&sktex, GPU_LINEAR, GPU_LINEAR);
	C3D_TexSetWrap(&sktex, GPU_CLAMP_TO_EDGE, GPU_CLAMP_TO_EDGE);
        d7gfx::Texture skktex;
        skktex.setTextuere(&sktex);
        
	    C3D_TexEnv* env = C3D_GetTexEnv(0);
	    C3D_TexEnvInit(env);
	    C3D_TexEnvSrc(env, C3D_Both, GPU_TEXTURE0, GPU_TEVSRC::GPU_CONSTANT, GPU_TEVSRC::GPU_CONSTANT);
	    C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);
        C3D_TexBind(0, &sktex);
    }
} /* d7gfx */
