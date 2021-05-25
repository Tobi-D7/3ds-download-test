

#include <3ds.h>
#include <string>
namespace RenderD7{
class Sound {
public:
	Sound(const std::string& path, int channel = 1, bool toloop = false);
	~Sound();
	void play();
	void stop();

private:
	u32 dataSize;
	ndspWaveBuf waveBuf;
	u8* data = NULL;
	int chnl;
};

}