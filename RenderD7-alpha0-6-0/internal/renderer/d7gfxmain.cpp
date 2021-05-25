#include "d7gfxmain.hpp"
#include <3ds.h>
#include <curl/curl.h>
#include <citro3d.h>
#include <cstring>
#include <malloc.h>

bool m_running;

namespace d7gfx {
    void InitApp(){
            m_running = true;
            printf("Success!");
            printf("Success!");
            aptInit();
            printf("Success!");
            cfguInit();
            printf("Success!");
            ptmuInit();
            printf("Success!");
            acInit();
            printf("Success!");
            romfsInit();
            printf("Success!");
            archiveMountSdmc();
            //mcuHwcInit();
            printf("Success!");
            gfxInitDefault();
            printf("Success!");
            C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
            printf("Success!");
            //Result res;
            //res = ndspInit();
            //if (!res) {
            //    d7gfx::priv::ndsp::initialized = true;
            //}

            srvInit();
            printf("Success!");
            srvGetServiceHandle(&d7gfx::priv::somethingidk::ptmsysmHandle, "ptm:sysm");
            printf("Success!");
            srvExit();
            printf("Success!");

            d7gfx::priv::somethingidk::socubuf = (u32*) memalign(0x1000, 0x100000);
            if (d7gfx::priv::somethingidk::socubuf) {
                if (!R_FAILED(socInit(d7gfx::priv::somethingidk::socubuf, 0x100000))) {
                    d7gfx::priv::somethingidk::socuInitialized = true;
                }
            }
            printf("Success!");
            osSetSpeedupEnable(true);
            printf("Success!");
    }

    void ExitApp() {
        socExit();
        //if (d7gfx::priv::ndsp::initialized) ndspExit();
        C3D_Fini();
        gfxExit();
        mcuHwcExit();
        romfsExit();
        acExit();
        ptmuExit();
        cfguExit();
        aptExit();
    }

    bool Mainloop() {
        if (!aptMainLoop()) return false;

        hidScanInput(); // scan input since this gets called every frame
        //C3D_FrameEnd(0);
        return m_running;
    }
}