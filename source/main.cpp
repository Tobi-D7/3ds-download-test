#include <3ds.h>
#include <iostream>

#include "gui.hpp"
#include "download.hpp"

int main ()
{
    //main
    Gui::init();
    gfxInitDefault();
    romfsInit();
    cfguInit();
    consoleInit(GFX_TOP, NULL);
    printf("D7-download-console:");
    downloadToFile("https://github.com/NPI-D7/nightlys/raw/master/builds/3ds-download-test/3ds-download-test.3dsx", "sdmc:/3ds-download-test.3dsx");
    

    while (aptMainLoop())
    {
        gspWaitForVBlank();
		gfxSwapBuffers();
        hidScanInput();
        u32 hDown = hidKeysDown();
        
        if (hDown & KEY_START)
        {
            break;
        }
    }
    Gui::exit();
    gfxExit();
    romfsExit();
    cfguExit();

}
