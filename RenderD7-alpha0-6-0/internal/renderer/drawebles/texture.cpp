#include <cstring>
#include <fstream>
#include <png.h>
#include <vector>
#include "texture.hpp"

namespace d7gfx {
    Texture::Texture() :
    m_width(0),
    m_height(0),
    m_path(""),
    m_texture(nullptr) { /* do nothing */ }

    Texture::Texture(d7gfx::Texture& t_texture) {
        operator=(t_texture);
    }

    Texture::~Texture() {
        if (m_texture != nullptr) {
            unloadImage(m_image);
        }
    }

    bool Texture::loadFromFile(const std::string& t_filename) {
        FILE* fp = fopen(t_filename.c_str(), "rb");
        return loadPng(fp);
    }

    bool Texture::loadFromBuffer(const void* t_buffer) {
        return loadPng(t_buffer, true);
    }

    int Texture::getWidth() {
        return m_width;
    }

    int Texture::getHeight() {
        return m_height;
    }

    int Texture::getCalculatedWidth() {
        return getNextPow2(m_width);
    }

    int Texture::getCalculatedHeight() {
        return getNextPow2(m_height);
    }

    std::string Texture::getPath() {
        return m_path;
    }

    C3D_Tex* Texture::getTexture() {
        return m_texture;
    }

    Tex3DS_SubTexture& Texture::getSubtexture() {
        return m_subtexture;
    }

   C2D_Image& Texture::getImage() {
       return m_image;
   }

    d7gfx::Texture& Texture::operator=(d7gfx::Texture& rhs) {
        if(this == &rhs) return *this;

        m_width = rhs.getWidth();
        m_height = rhs.getHeight();
        m_path = rhs.getPath();

        if (m_texture) C3D_TexDelete(m_texture);

        if (rhs.getTexture() != nullptr) {
            m_texture = static_cast<C3D_Tex*>(malloc(sizeof(C3D_Tex)));
            C3D_TexInit(m_texture, getNextPow2(rhs.getTexture()->width), getNextPow2(rhs.getTexture()->height), GPU_RGBA8);
            memcpy(m_texture, rhs.getTexture(), sizeof(C3D_Tex));
        }

        return *this;
    }

    // private methods
    bool Texture::loadPng(const void* t_fp, bool t_buffer) {
        png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

        png_infop info = png_create_info_struct(png);

        if(setjmp(png_jmpbuf(png)))
        {
            png_destroy_read_struct(&png, &info, NULL);
            return false;
        }

        if (t_buffer) {
            png_set_read_fn(png, (png_voidp) t_fp,
                            [](png_structp ptr, png_bytep data, png_size_t len) {
                                unsigned int* addr = (unsigned int*) png_get_io_ptr(ptr);
                                memcpy(data, (void*) *addr, len);
                                *addr += len;
                            });
        } else {
            png_set_read_fn(png, (png_voidp) t_fp,
                            [](png_structp ptr, png_bytep data, png_size_t len) {
                                fread(data, 1, len, (FILE*) png_get_io_ptr(ptr));
                            });
        }

        png_read_info(png, info);

        m_width = png_get_image_width(png, info);
        m_height = png_get_image_height(png, info);

        png_byte color_type = png_get_color_type(png, info);
        png_byte bit_depth  = png_get_bit_depth(png, info);

        // Read any color_type into 8bit depth, ABGR format.
        // See http://www.libpng.org/pub/png/libpng-manual.txt

        if(bit_depth == 16)
            png_set_strip_16(png);

        if(color_type == PNG_COLOR_TYPE_PALETTE)
            png_set_palette_to_rgb(png);

        // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
        if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
            png_set_expand_gray_1_2_4_to_8(png);

        if(png_get_valid(png, info, PNG_INFO_tRNS))
            png_set_tRNS_to_alpha(png);

        // if color_type doesn't have an alpha channel, fill it with 0xff.
        if(color_type == PNG_COLOR_TYPE_RGB ||
           color_type == PNG_COLOR_TYPE_GRAY ||
           color_type == PNG_COLOR_TYPE_PALETTE)
            png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

        if(color_type == PNG_COLOR_TYPE_GRAY ||
           color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
            png_set_gray_to_rgb(png);

        // output ABGR
        png_set_bgr(png);
        png_set_swap_alpha(png);

        png_read_update_info(png, info);

        png_bytep* row_pointers = static_cast<png_byte**>(malloc(sizeof(png_bytep) * m_height));
        for(int y = 0; y < m_height; y++) {
            row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
        }

        png_read_image(png, row_pointers);

        if (!t_buffer) fclose((FILE*) t_fp);
        png_destroy_read_struct(&png, &info, NULL);

        unloadImage(m_image);

        m_texture = static_cast<C3D_Tex*>(malloc(sizeof(C3D_Tex)));
        m_image.tex = m_texture;
        m_subtexture.width = m_width;
        m_subtexture.height = m_height;
        m_subtexture.left = 0.0f;
        m_subtexture.top = 1.0f;
        m_subtexture.right = m_width / (float) getNextPow2(m_width);
        m_subtexture.bottom = 1.0 - (m_height / (float) getNextPow2(m_height));
        m_image.subtex = &m_subtexture;

        C3D_TexInit(m_image.tex, getNextPow2(m_width), getNextPow2(m_height), GPU_RGBA8);

        memset(m_image.tex->data, 0, m_image.tex->size);

        for(int j = 0; j < m_height; j++) {
            png_bytep row = row_pointers[j];
            for(int i = 0; i < m_width; i++) {
                png_bytep px = &(row[i * 4]);
                u32 dst = ((((j >> 3) * (getNextPow2(m_height) >> 3) + (i >> 3)) << 6) + ((i & 1) | ((j & 1) << 1) | ((i & 2) << 1) | ((j & 2) << 2) | ((i & 4) << 2) | ((j & 4) << 3))) * 4;

                memcpy(static_cast<char*>(m_image.tex->data) + dst, px, sizeof(u32));
            }
        }

        // C3D_TexSetFilter(m_texture, GPU_LINEAR, GPU_LINEAR);
        C3D_TexSetWrap(m_image.tex, GPU_CLAMP_TO_BORDER, GPU_CLAMP_TO_BORDER);
        m_image.tex->border = 0xFFFFFFFF;

        return true;
    }

    inline void Texture::unloadImage(C2D_Image t_image) {
        if (t_image.tex) C3D_TexDelete(t_image.tex);
    }

    inline u32 Texture::getNextPow2(u32 v) {
        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v++;
        return v >= 64 ? v : 64;
    }
} /* d7gfx */