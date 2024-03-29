#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>
#include <memory>
#include <stack>
#include <string>
#include <functional>
#include <vector>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <cstring>
#include <sys/stat.h>
#include "external/lodepng.h"
#include "sound.hpp"
#include <codecvt>
#include "lang.hpp"

extern "C"
{
    #include "external/fs.h"
}
#define RENDERD7VSTRING "0.6.0"
#define DEFAULT_CENTER 0.5f

extern C3D_RenderTarget* Top;
extern C3D_RenderTarget* TopRight;
extern C3D_RenderTarget* Bottom;

extern u32 d7_hDown;
extern u32 d7_hHeld;
extern u32 d7_hUp;
extern touchPosition d7_touch;

extern std::string dspststus;

namespace RenderD7
{
    void OnScreen(C3D_RenderTarget *target);
    class Sheet
    {
        public:
        Sheet();
        ~Sheet();
        Result Load(const char *path);
        void Free();
        C2D_SpriteSheet spritesheet;
        
    };
    class Image
    {
        public:
        void LoadPng(const std::string path);
        void LoadPFromBuffer(const std::vector<u8> &buffer);
        bool Draw(float x, float y, float scaleX = 1.0f, float scaleY = 1.0f);
        bool DrawCentered(float x = 0, float y = 0, float screenw = 400, float screenh = 240);
        std::string GetPath();
        inline C2D_Image Get(){return this->img;}
        C2D_Image img;
        bool loadet = false;
        private:
        std::string path;
    };

    

    class Sprite
    {
        public:
        Sprite();
        ~Sprite();
        void FromSheet(RenderD7::Sheet *sheet, size_t index);
        void FromImage(RenderD7::Image img);
        void FromC2DImage(C2D_Image img);
        void FromSprite(RenderD7::Sprite *sprite);
        bool Draw();
        void SetCenter(float x, float y);
        void SetPos(float x, float y);
        void SetScale(float x, float y);
        void SetRotation(float rotation);
        void Rotate(float speed);
        float getWidth();
        float getHeigh();
        float getPosX();
        float getPosY();
        inline C2D_Sprite GetSprite(){return this->sprite;}
        private:
        C2D_Sprite sprite;
        C2D_ImageTint tint;
        
    };
    class Scene {
    public:
        static std::stack<std::unique_ptr<Scene>> scenes;
    	virtual ~Scene() {}
    	virtual void Logic(u32 hDown, u32 hHeld, u32 hUp, touchPosition touch) = 0;
    	virtual void Draw() const = 0;
        static void Load(std::unique_ptr<Scene> scene);
        static void Back();
        static void doDraw();
        static void doLogic(u32 hDown, u32 hHeld, u32 hUp, touchPosition touch);

    };
    namespace Color
    {
        u32 Hex(const std::string color, u8 a = 255);
    }
    bool DrawImageFromSheet(RenderD7::Sheet* sheet, size_t index, float x, float y, float scaleX = 1.0, float scaleY = 1.0);
    namespace Error
    {
        void DisplayError(std::string toptext, std::string errortext);
        void DisplayFatalError(std::string toptext, std::string errortext);
    }
    namespace Init
    {
        Result Main();
        Result RenderHD();
        void NdspFirm(bool useit = false);
    }
    namespace Exit
    {
        void Main();
        void NdspFirm();
    }
    namespace Msg
    {
        void Display(std::string titletxt, std::string subtext, C3D_RenderTarget *target);
        void DisplayWithProgress(std::string titletext, std::string subtext, float current, float total, u32 prgbarcolor);
    }
    
    namespace FS
    {
        bool FileExist(const std::string& path);
    }
    void TakeScreenshot(const char* path);
    bool IsNdspInit();
    void SetupLog(void);
    std::string GetFramerate();
    bool MainLoop();
    void ExitApp();
    u8 GetConsoleModel();
    void ClearTextBufs(void);
   
    bool DrawRect(float x, float y, float w, float h, u32 color);
    void DrawTextCentered(float x, float y, float size, u32 color, std::string Text, int maxWidth = 0, int maxHeight = 0, C2D_Font fnt = nullptr);
	void DrawText(float x, float y, float size, u32 color, std::string Text, int maxWidth = 0, int maxHeight = 0, C2D_Font fnt = nullptr);
	float GetTextWidth(float size, std::string Text, C2D_Font fnt = nullptr);
	void GetTextSize(float size, float *width, float *height, std::string Text, C2D_Font fnt = nullptr);
	float GetTextHeight(float size, std::string Text, C2D_Font fnt = nullptr);
    Result loadFont(C2D_Font &fnt, const char * Path = "");
	Result unloadFont(C2D_Font &fnt);
    bool DrawCircle(float x, float y, float radius, u32 color);
    bool DrawImage(C2D_Image img, float x, float y, float scaleX = 1.0f, float scaleY = 1.0f);
    

    class SpriteSheetAnimation : public RenderD7::Sprite
    {
        
        public:
        SpriteSheetAnimation();
        ~SpriteSheetAnimation();
        void Setup(RenderD7::Sheet *sheet, size_t imagecount, size_t startimage, float frame_begin, float frame_finish);
        void Play(float timespeed);
        private:
        size_t images;
        size_t imgs = 0;
        float D_totaltime;
        RenderD7::Sheet *sheet;
        float time;

    };
    class SpriteAnimation : public RenderD7::Sprite
    {
        
        public:
        SpriteAnimation();
        ~SpriteAnimation();
        void SetupIndex(RenderD7::Sheet *sheet, std::vector<size_t> index, float frame_begin, float frame_finish);
        void SetupFromImgs(std::vector<RenderD7::Image> images_names, float begin, float end);
        void SetupFromSprites(std::vector<RenderD7::Sprite> sprites, float frames);
        void Play(float timespeed);
        bool DrawAnimatet();
        private:
        std::vector<RenderD7::Image> imagesnames;
        std::vector<RenderD7::Sprite> anmsprites;
        RenderD7::Image img;
        int imagesl = 0;
        
        size_t totalimages;
        size_t imgs = 0;
        float D_totaltime;
        RenderD7::Sheet *sheet;
        float time;

    };
    struct TObject
    {
        int x;
        int y;
        int w;
        int h;
        std::string text = "";
        float correctx = 0; 
        float correcty = 0;
        float txtsize = 0.7f;
        
    };
    void DrawTObjects(std::vector<RenderD7::TObject> tobjects, u32 color, u32 txtcolor,  int selection = -1, u32 selbgcolor = RenderD7::Color::Hex("#2D98AF"), u32 selcolor = RenderD7::Color::Hex("#000000"));
    void DrawSTObject(std::vector<RenderD7::TObject> tobject, int tobjectindex, u32 color, u32 txtcolor);
    bool touchTObj(touchPosition touch, RenderD7::TObject button);
    
    struct DirContent
    {
        std::string name;
        std::string path;
        bool isDir;
    };

    bool NameIsEndingWith(const std::string &name, const std::vector<std::string> &extensions);
    void GetDirContentsExt(std::vector<RenderD7::DirContent> &dircontent, const std::vector<std::string> &extensions);
    void GetDirContents(std::vector<RenderD7::DirContent> &dircontent);
    void DrawDebugInfo();
    
}