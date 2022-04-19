#include <d7gfx-driver.hpp>
#include <renderd7.hpp>
//1
bool _dcl = true;
bool _mdl = false;

std::string DrC = "";
int main()
{
    d7gfx::InitApp();  
    consoleInit(GFX_BOTTOM, NULL);
    d7gfx::Color sky(122, 145, 255); 
    d7gfx::Renderer renderer(true);
    d7gfx::Model modl;
    d7gfx::Texture modltex;
    d7gfx::Texture sKtt;
    d7gfx::Text dcount;
    d7gfx::Text fcount;
    fcount.setPosition(0, 30);
    d7gfx::Camera &cam = renderer.getCamera(d7gfx::RenderContext::ScreenTarget::Top);
    d7gfx::Cube cube(1, 1, 1);
    d7gfx::SkyBox sk(50);
    d7gfx::Plane pln(400, 400);
    d7gfx::Color c1(25, 100, 244);
    if (RenderD7::FS::FileExist("sdmc:/mdl.obj")) _mdl = true;
    if (_mdl)
    {
        modl.loadFromFile("sdmc:/mdl.obj");
        modltex.loadFromFile("sdmc:/mdl.png");
        sKtt.loadFromFile("romfs:/gfx/skybox.png");
        sk.bindTexture(sKtt);
        modl.setPosition(20, 0, 20);
        modl.bindTexture(modltex);
    }
    d7gfx::Rect rec(0, 0, 200, 200, c1);
    d7gfx::Texture ptex;
    ptex.loadFromFile("romfs:/gfx/texture.png");

    cube.bindTexture(ptex);
    while (d7gfx::Mainloop())
    {
        dcount.setText(DrC);
        fcount.setText(RenderD7::GetFramerate());
        hidScanInput();
      /*  printf("\x1b[5;1HFramerate: %s\x1b[K", RenderD7::GetFramerate().c_str());
        printf("\x1b[6;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime()*6.0f);
	printf("\x1b[7;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime()*6.0f);
	printf("\x1b[8;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage()*100.0f);
*/
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
        
        if(d7_hHeld & KEY_R) {
            float dirY = std::sin(cam.getYaw(true)) * 0.2;
            cam.moveY(dirY);
        }
        if(d7_hHeld & KEY_L) {
            float dirY = -std::sin(cam.getYaw(true)) * 0.2;
            cam.moveY(dirY);
        }
        if(d7_hDown & KEY_START)break;
        if(d7_hDown & KEY_A) _dcl = true;
        if(d7_hDown & KEY_B) _dcl = false;
        if(d7_hHeld & KEY_CPAD_DOWN) {
            float dirX = std::sin(cam.getYaw(true)) * 0.2;
            float dirZ = -std::cos(cam.getYaw(true)) * 0.2;
            cam.moveX(dirX);
            cam.moveZ(dirZ);
        }
        if(d7_hHeld & KEY_CPAD_UP) {
            float dirX = -std::sin(cam.getYaw(true)) * 0.2;
            float dirZ = std::cos(cam.getYaw(true)) * 0.2;
            cam.moveX(dirX);
            cam.moveZ(dirZ);
        }
        if(d7_hHeld & KEY_CPAD_RIGHT) {
            float dirX = -std::cos(cam.getYaw(true)) * 0.2;
            float dirZ = -std::sin(cam.getYaw(true)) * 0.2;
            cam.moveX(dirX);
            cam.moveZ(dirZ);
        }
        if(d7_hHeld & KEY_CPAD_LEFT) {
            float dirX = std::cos(cam.getYaw(true)) * 0.2;
            float dirZ = std::sin(cam.getYaw(true)) * 0.2;
            cam.moveX(dirX);
            cam.moveZ(dirZ);
        }

        cube.rotatePitch(1);
        cube.rotateYaw(1);
        cube.rotateRoll(1);
        modl.rotatePitch(1);
        modl.rotateYaw(1);
        modl.rotateRoll(1);
        renderer.drawTop(cube, d7gfx::RenderContext::Mode::Spatial);
        renderer.drawTop(pln, d7gfx::RenderContext::Mode::Spatial);
       // renderer.drawTop(mark7, d7gfx::RenderContext::Mode::Spatial);
       // renderer.drawBottom(cube, d7gfx::RenderContext::Mode::Spatial);
       // renderer.drawBottom(pln, d7gfx::RenderContext::Mode::Spatial);
        renderer.drawTop(sk, d7gfx::RenderContext::Mode::Spatial);
        renderer.drawTop(dcount, d7gfx::RenderContext::Mode::Flat);
        renderer.drawTop(fcount, d7gfx::RenderContext::Mode::Flat);



        //renderer.drawBottom(rec);
        if (_mdl) renderer.drawTop(modl, d7gfx::RenderContext::Mode::Spatial);
  
       // renderer.drawTop(spr1);
        //renderer.drawBottom(skyb, d7gfx::RenderContext::Mode::Spatial);
        //renderer.drawBottom(cube, d7gfx::RenderContext::Mode::Spatial);
        renderer.Render(_dcl);
        DrC = std::to_string(renderer.GetDrawCalls());
        renderer.setClearColor(sky);
    }
    d7gfx::ExitApp();
    
}
