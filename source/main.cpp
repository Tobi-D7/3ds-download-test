#include <3ds.h>
#include <download.hpp>

int main()
{
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, NULL);
    downloadToFile("https://github.com/Creckeryop/3dsCookieClicker/archive/refs/heads/master.zip", "sdmc:/CC.zip");
    while (aptMainLoop())
    {
    
    }
    
}
