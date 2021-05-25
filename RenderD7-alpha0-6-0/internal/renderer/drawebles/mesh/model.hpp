
#pragma once
#include "mesh.hpp"

namespace d7gfx {
    enum mdlType
    {
        OBJ,
        MD2
    };
    class Model: public d7gfx::Mesh {
    public:
        bool loadFromFile(const std::string& t_filename);
    };
} /* d7gfx */