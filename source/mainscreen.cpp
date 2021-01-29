#include "mainscreen.hpp"
#include "download.hpp"
#include "colors.hpp"
#include "msg.hpp"
extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern bool exiting;



void MMMENU::Draw(void) const {
    

    GFX::DrawTop();
    Gui::DrawStringCentered(0, 120, 0.8f, WHITE, "Press A to download!", 400);
    GFX::DrawBotom();

}
void MMMENU::Logic(u32 hDown, u32 hHeld, touchPosition touch){

    if (hDown & KEY_START)
    {
        exiting = true;
    }
    if (hDown & KEY_A)
    {
        MSG::DisplayMsg(2, 4);
        downloadToFile("https://github.com/NPI-D7/nightlys/raw/master/builds/3ds-download-test/3ds-download-test.3dsx", "sdmc:/3ds-download-test.3dsx");
    }
}
