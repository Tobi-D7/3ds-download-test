#include <3ds.h>
#include <download.hpp>

extern curl_off_t downloadTotal;
extern curl_off_t downloadNow;

bool dl = true;
int main()
{
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, NULL);
    char str[256];
    while (aptMainLoop())
    {
         if (dl) 
         {
                 downloadToFile("https://github.com/Creckeryop/3dsCookieClicker/archive/refs/heads/master.zip", "sdmc:/CC.zip");
         } 
         snprintf(str, sizeof(str), (%.2f%%), ((float)downloadNow/(float)downloadTotal) * 100.0f); 
         pritf(str);
    }
    
}
