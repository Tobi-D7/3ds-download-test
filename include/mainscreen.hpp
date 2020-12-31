#include "gui.hpp"
#include "gfx.hpp"
#include "Init.hpp"

class MMMENU : public Screen {

    public:
            void Draw(void) const override;
	        void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;

    private:

};