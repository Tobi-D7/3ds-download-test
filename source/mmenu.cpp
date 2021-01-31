#include "mmenu.hpp"

extern bool exiting;

void MMENU::Draw(void) const
{
    UI::DrawonScreen(Top);

}

void MMENU::Logic(u32 hDown, u32 hHeld, touchPosition touch)
{
    if (hDown && KEY_B) exiting = true;
}