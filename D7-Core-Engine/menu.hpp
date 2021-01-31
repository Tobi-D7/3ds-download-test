#include "includes.hpp"

#include "structs.hpp"

extern C3D_RenderTarget *Top, *TopRight, *Bottom;
extern bool fadeout, fadein, fadeout2, fadein2;
extern int fadealpha, fadecolor;

class Menu{
    virtual ~Menu() {}
	virtual void Logic(u32 hDown, u32 hHeld, touchPosition touch) = 0;
	virtual void Draw() const = 0;
};