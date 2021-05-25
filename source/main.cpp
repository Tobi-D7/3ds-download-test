#include <3ds.h>
#include "d7gfx-driver.hpp"
#include "renderd7.hpp"
using namespace d7gfx;



int main()
{
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, NULL);
    InitApp();
    printf("Hello!");
    Renderer renderer(false);
    HTTPClient client;
    Color c1(255, 255, 255);
    Color c2(255, 155, 255);
    Rect rect(30, 100, 102, 30, c1);
    Rect rect2(31, 101, 0, 28, c2);
    bool finished = false;
    
    printf("Hello!");
    client.downloadToFileAsync("https://github.com/NPI-D7/nightlys/raw/master/builds/D7-Menu/D7-Menu.3dsx", "sdmc:/D7-Menu.3dsx");
    while (Mainloop())
    {
        Text text;
        text.setText("Hello");
        if (client.isFinished() && !finished) {
            finished = true; // we only want this to execute once

            if (client.getStatus() == client.Status::OK) {
                printf("Download finished!");
            } else {
                printf("The request failed due to an error");
            }
        }
        else if (client.getStatus() == client.Status::Failed)
        {
            break;
        } else {
            rect2.setWidth((int)(((float)client.getProgress() / (float)client.getTotalSize()) * 100.0f));
        }
        renderer.drawTop(rect);
        renderer.drawTop(rect2);
        renderer.drawTop(text);
        renderer.Render(true);
        C3D_FrameEnd(0);
    }
    ExitApp();
}
