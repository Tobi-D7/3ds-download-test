#include "animation.hpp"
#include "gui.hpp"
#include "colors.hpp"

void Animation::DrawProgressBar(u64 currentProgress, u64 totalProgress) {
	Gui::Draw_Rect(30, 120, 340, 30, BLUE);
	Gui::Draw_Rect(31, 121, (int)(((float)currentProgress / (float)totalProgress) * 338.0f), 28, WHITE);
}