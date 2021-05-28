#include <d7gfx-driver.hpp>

int main()
{
    d7gfx::InitApp();   
    d7gfx::Renderer renderer(true);
    d7gfx::Camera &cam = renderer.getCamera(d7gfx::RenderContext::ScreenTarget::Bottom);
    d7gfx::Cube cube(2, 3, 4);
    d7gfx::Color c1(25, 100, 244);
    d7gfx::Rect rec(0, 0, 200, 200, c1);
    d7gfx::Texture tex1;
    tex1.loadFromFile("romfs:/gfx/d7.png");
    d7gfx::Sprite spr1;
    spr1.setTexture(tex1);
    //consoleInit(GFX_BOTTOM, NULL);
    //printf("success!");
    while (d7gfx::Mainloop())
    {
        hidScanInput();
        u32 d7_hDown = hidKeysDown();
        u32 d7_hHeld = hidKeysHeld();
        if(d7_hHeld & KEY_CSTICK_DOWN) 
        {
            cam.rotatePitch(1);
            if (90 < cam.getPitch())
                cam.setPitch(90);
        }
        if(d7_hHeld & KEY_CSTICK_UP) 
        {
            cam.rotatePitch(-1);
            if(-90 > cam.getPitch())
                cam.setPitch(-90);
        }
        if(d7_hHeld & KEY_CSTICK_RIGHT) cam.rotateYaw(1);
        if(d7_hHeld & KEY_CSTICK_LEFT) cam.rotateYaw(-1);


        if(d7_hDown & KEY_START)break;
        if(d7_hHeld & KEY_CPAD_DOWN) {
            float dirX = std::sin(cam.getYaw(true)) * 0.03;
            float dirZ = -std::cos(cam.getYaw(true)) * 0.03;
            cam.moveX(dirX);
            cam.moveZ(dirZ);
        }
        if(d7_hHeld & KEY_CPAD_UP) {
            float dirX = -std::sin(cam.getYaw(true)) * 0.03;
            float dirZ = std::cos(cam.getYaw(true)) * 0.03;
            cam.moveX(dirX);
            cam.moveZ(dirZ);
        }
        if(d7_hHeld & KEY_CPAD_RIGHT) {
            float dirX = -std::cos(cam.getYaw(true)) * 0.03;
            float dirZ = -std::sin(cam.getYaw(true)) * 0.03;
            cam.moveX(dirX);
            cam.moveZ(dirZ);
        }
        if(d7_hHeld & KEY_CPAD_LEFT) {
            float dirX = std::cos(cam.getYaw(true)) * 0.03;
            float dirZ = std::sin(cam.getYaw(true)) * 0.03;
            cam.moveX(dirX);
            cam.moveZ(dirZ);
        }


        spr1.setScale(0.1,0.1);
        renderer.drawTop(rec);
        renderer.drawTop(spr1);
        renderer.drawBottom(cube, d7gfx::RenderContext::Mode::Spatial);
        renderer.Render(true);
    }
    d7gfx::ExitApp();
    
}