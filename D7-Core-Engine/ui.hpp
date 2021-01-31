#include "includes.hpp"
#include "menu.hpp"

namespace UI {
    Result init(void);
    void fini(void);
    Result reinit(void);
    void clearTextbuf(void);
    namespace T3X {
        Result loadSpriteSheet(const char * sheetpath, C2D_SpriteSheet &spritesheet);
        Result unloadSpriteSheet(C2D_SpriteSheet &spritesheet);
        void DrawSprite(C2D_SpriteSheet spritesheet, size_t imgindex, int x = 0, int y = 0, float Xscale = 1, float Yscale = 1);
    }
    Result loadFont(C2D_Font &bcfnt, const char *path = "");
    Result unloadFont(C2D_Font &bcfnt);
    void DrawonScreen(C3D_RenderTarget * screen);
}