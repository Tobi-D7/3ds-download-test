#pragma once
#include <citro3d.h>
#include "rendercontext.hpp"

namespace d7gfx {
   
    class Drawable {
    public:
        virtual void draw(d7gfx::RenderContext t_context) = 0;
    };
} /* d7gfx */