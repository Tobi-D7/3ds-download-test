#include "gfx.hpp"

void GFX::DrawTop(void)
{
    Gui::ScreenDraw(Top);
    Gui::Draw_Rect(0, 0, 20, 240, C2D_Color32(0, 255, 100, 80));
    Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(255, 255, 0, 255));
}
void GFX::DrawBottom(void)
{
    Gui::ScreenDraw(Bottom);
}