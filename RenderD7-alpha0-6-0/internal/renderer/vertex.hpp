#pragma once
#include "color.hpp"
namespace d7gfx
{
    struct Vector2f
    {
        float u;
        float v;
    };
    struct Vector3f
    {
        float x;
        float y;
        float z;
    };
    struct ColoredVertex {
        Vector3f position;
        d7gfx::Color color;
    };
    struct TexturedVertex
    {
        Vector3f position;       
        Vector2f textureCoords;
    };
    
}