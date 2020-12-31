#include "gui.hpp"
#include "gfx.hpp"
#include "Init.hpp"
#include "structs.hpp"

#include <vector>

class MMMENU : public Screen {

    public:
            void Draw(void) const override;
	        void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;

    private:
    std::vector<Structs::ButtonPos> mainButtons = {
		{90, 40, 140, 35}, 

    };    


};