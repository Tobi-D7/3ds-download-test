#include "Init.hpp"
#include "ui.hpp"

bool exiting = false;




bool touching(touchPosition touch, Structs::ButtonPos button) {
	if (touch.px >= button.x && touch.px <= (button.x + button.w) && touch.py >= button.y && touch.py <= (button.y + button.h))	return true;
	else	return false;
}

Result Init::Initialize() {
	fadealpha = 255;
	fadein = true;
	UI::init();
	gfxInitDefault();
	romfsInit();
	cfguInit();
	osSetSpeedupEnable(true);
	return 0;
}

Result Init::MainLoop() {
	Init::Initialize();

	while (aptMainLoop()) {
	
		hidScanInput();
		u32 hDown = hidKeysDown();
		u32 hHeld = hidKeysHeld();
		touchPosition touch;
		hidTouchRead(&touch);

		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C3D_FrameEnd(0);
		if (exiting) {
			if (!fadeout)	break;
		}

	}

	Init::Exit();
	return 0;
}

Result Init::Exit() {
	UI::fini();
	gfxExit();
	cfguExit();
	romfsExit();
	return 0;
	//XDDD
}