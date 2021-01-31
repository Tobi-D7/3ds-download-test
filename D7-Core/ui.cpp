#include "ui.hpp"

C3D_RenderTarget *Top, *TopRight, *Bottom;

C2D_TextBuf TextBuf;
C2D_Font Font;
std::unique_ptr<Menu> usedMenu, tempMenu;
std::stack<std::unique_ptr<Menu>> Menus;
bool currentMenu = false;
bool fadeout = false, fadein = false, fadeout2 = false, fadein2 = false;
int fadealpha = 0;
int fadecolor = 0;

Result UI::init(void)
{
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	Top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	TopRight = C2D_CreateScreenTarget(GFX_TOP, GFX_RIGHT);
	Bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
	TextBuf = C2D_TextBufNew(4096);
	Font = C2D_FontLoadSystem(CFG_REGION_USA);
	return 0;
}
void UI::fini(void)
{
    C2D_TextBufDelete(TextBuf);
	C2D_Fini();
	C3D_Fini();
	if (usedMenu) usedMenu = nullptr;
}
Result UI::T3X::loadSpriteSheet(const char * sheetpath, C2D_SpriteSheet &spritesheet)
{
	if (access(sheetpath, F_OK) == 0) spritesheet = C2D_SpriteSheetLoad(sheetpath);
	return 0;
}
Result UI::T3X::unloadSpriteSheet(C2D_SpriteSheet &spritesheet)
{
	if(spritesheet) C2D_SpriteSheetFree(spritesheet);
	return 0;
}
void UI::T3X::DrawSprite(C2D_SpriteSheet spritesheet, size_t imgindex, int x, int y, float Xscale, float Yscale)
{
	if (spritesheet) {
		if (C2D_SpriteSheetCount(spritesheet) >= imgindex) {
			C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, imgindex), x, y, 0.5f, nullptr, Xscale, Yscale);
		}
	}
}
void UI::clearTextbuf(void)
{
	C2D_TextBufClear(TextBuf);
}
Result UI::loadFont(C2D_Font &bcfnt, const char *path)
{
	if (access(path, F_OK) == 0) bcfnt = C2D_FontLoad(path);
	return 0;
}
Result UI::unloadFont(C2D_Font &bcfnt)
{
	if (bcfnt) C2D_FontFree(bcfnt);
}
Result UI::reinit(void)
{
	C2D_TextBufDelete(TextBuf);
	C2D_Fini();
	C3D_Fini();

	return UI::init();
}
void UI::DrawonScreen(C3D_RenderTarget * screen)
{
	C2D_SceneBegin(screen);
	currentMenu = (screen == Top || screen == TopRight) ? 1 : 0;
}
void UI::loadMenu(std::unique_ptr<Menu> menu, bool fade, bool stack)
{
	tempMenu = std::move(menu);
}