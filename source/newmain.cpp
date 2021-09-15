#include <d7gfx-driver.hpp>
//1
bool _dcl = true;
int main()
{
    d7gfx::InitApp();  
    consoleInit(GFX_BOTTOM, NULL);
    d7gfx::Color sky(122, 145, 221); 
    d7gfx::Renderer renderer(true);
    d7gfx::Camera &cam = renderer.getCamera(d7gfx::RenderContext::ScreenTarget::Top);
    d7gfx::Cube cube(1, 1, 1);
    d7gfx::Color c1(25, 100, 244);
    d7gfx::Model MdL;
    d7gfx::Model mark7;
    d7gfx::Model tpt;
    tpt.loadFromFile("romfs:/gfx/teapot.obj");
    MdL.loadFromFile("romfs:/gfx/playcoin.obj");
    mark7.loadFromFile("romfs:/gfx/moon.obj");
    MdL.setPosition(-2, 0, 0);
   // mark7.setScale(0.2, 0.2, 0.2);
   // d7gfx::Rect rec(0, 0, 200, 200, c1);
    d7gfx::Texture tex1;
    tex1.loadFromFile("romfs:/gfx/moon.png");
    d7gfx::Texture ptex;
    ptex.loadFromFile("romfs:/gfx/PlayCoin.png");
    mark7.bindTexture(tex1);
    MdL.bindTexture(ptex);
    d7gfx::Sprite spr1;
    //d7gfx::SkyBox skyb(1.0f);
    tpt.setPosition(5, 0, 0);
    //skyb.SetSkyBoxTex("romfs:/gfx/skybox.t3x");
    //spr1.setTexture(tex1);
    //consoleInit(GFX_BOTTOM, NULL);
    //printf("success!");
    mark7.setPosition(10, 0, 0);
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
        if(d7_hDown & KEY_A) _dcl = true;
        if(d7_hDown & KEY_B) _dcl = false;
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

        cube.rotatePitch(1);
        cube.rotateYaw(1);
        cube.rotateRoll(1);
        MdL.rotateYaw(1);
        tpt.rotateYaw(1);
        renderer.drawTop(MdL, d7gfx::RenderContext::Mode::Spatial);
       // spr1.setScale(0.1,0.1);
        renderer.drawTop(tpt, d7gfx::RenderContext::Mode::Spatial);
        renderer.drawTop(mark7, d7gfx::RenderContext::Mode::Spatial);


        //renderer.drawTop(rec);
       // renderer.drawTop(spr1);
        //renderer.drawBottom(skyb, d7gfx::RenderContext::Mode::Spatial);
        //renderer.drawBottom(cube, d7gfx::RenderContext::Mode::Spatial);
        renderer.Render(_dcl);
        renderer.setClearColor(sky);
    }
    d7gfx::ExitApp();
    
}
