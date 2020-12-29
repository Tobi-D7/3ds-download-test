#include <3ds.h>
#include <iostream>

#include "gui.hpp"

int main ()
{
    //main
    Gui::init();
    gfxInitDefault();
    romfsInit();
    cfguInit();
    

    while (aptMainLoop())
    {
        hidScanInput();
        u32 hDown = hidKeysDown();
        u32 hHeld = hidKeysHeld();

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