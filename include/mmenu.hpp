#include "ui.hpp"
#include "menu.hpp"

class MMENU : public Menu
{
    public:
        void Draw(void) const override;
	    void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
    private:
        //Nothing!
};