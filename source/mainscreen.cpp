#include "mainscreen.hpp"
#include "download.hpp"
extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern bool exiting;

void MMENU::Draw(void) const {

    GFX::DrawTop();
    Gui::DrawStringCentered(0, 120, 7f, C2D_Color32(0, 0, 0, 255), "Press A to download!", 400);

}
void MMENU::Logic(u32 hDown, u32 hHeld, touchPosition touch){

    if (hDown & KEY_START)
    {
        exiting = true;
    }
    if (hDown & KEX_A)
    {
        downloadToFile("https://github.com/NPI-D7/nightlys/raw/master/builds/3ds-download-test/3ds-download-test.3dsx", "sdmc:/3ds-download-test.3dsx");
    }
}