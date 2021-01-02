#include "mainscreen.hpp"
#include "download.hpp"
#include "colors.hpp"
extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern bool exiting;
D7::Color txtcolor;


void MMMENU::Draw(void) const {
    txtcolor.set(255, 255, 255, 255);

    GFX::DrawTop();
    Gui::DrawStringCentered(0, 120, 0.8f, txtcolor, "Press A to download!", 400);

}
void MMMENU::Logic(u32 hDown, u32 hHeld, touchPosition touch){

    if (hDown & KEY_START)
    {
        exiting = true;
    }
    if (hDown & KEY_A)
    {
        downloadToFile("https://github.com/NPI-D7/nightlys/raw/master/builds/3ds-download-test/3ds-download-test.3dsx", "sdmc:/3ds-download-test.3dsx");
    }
}
