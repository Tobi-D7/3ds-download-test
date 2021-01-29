#include "msg.hpp"
#include "colors.hpp"
#include "animation.hpp"
#include <citro3d.h>
#include <citro2d.h>
void MSG::DisplayMsg(u64 current, u64 total)
{
    Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, TRANSPARENT);
	C2D_TargetClear(Bottom, TRANSPARENT);

	GFX::DrawTop();
	Gui::DrawStringCentered(0, (240 - Gui::GetStringHeight(0.6f, "Text")) / 2, 0.6f, WHITE, "Text", 395, 0);
	Animation::DrawProgressBar(current, total);
	GFX::DrawBottom();
	C3D_FrameEnd(0);
}

void MSG::Msg(std::string Text)
{
    Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, TRANSPARENT);
	C2D_TargetClear(Bottom, TRANSPARENT);

	GFX::DrawTop();
	Gui::DrawStringCentered(0, (240 - Gui::GetStringHeight(0.6f, Text)) / 2, 0.6f, WHITE, Text, 395, 0);

	GFX::DrawBottom();
	C3D_FrameEnd(0);
}