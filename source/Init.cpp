#include "Init.hpp"
#include "structs.hpp"
#include "mainscreen.hpp"
#include "screenCommon.hpp"


bool exiting = false;




bool touching(touchPosition touch, Structs::ButtonPos button) {
	if (touch.px >= button.x && touch.px <= (button.x + button.w) && touch.py >= button.y && touch.py <= (button.y + button.h))	return true;
	else	return false;
}

Result Init::Initialize() {
	fadealpha = 255;
	fadein = true;

	gfxInitDefault();
	romfsInit();
	Gui::init();
	cfguInit();
	osSetSpeedupEnable(true);	
	
	Gui::setScreen(std::unique_ptr<MMMENU>(), false, false);
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
		Gui::DrawScreen(false);
		Gui::ScreenLogic(hDown, hHeld, touch, true, false);
		C3D_FrameEnd(0);
		if (exiting) {
			if (!fadeout)	break;
		}

		Gui::fadeEffects(6, 6, false);
	}

	Init::Exit();
	return 0;
}

Result Init::Exit() {
	Gui::exit();
	gfxExit();
	cfguExit();
	romfsExit();
	return 0;
}