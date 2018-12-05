#include "app/menu/widget.hpp"
#include "pge/draw.hpp"
#include "pge/image.hpp"
#include "pge/sound.hpp"
#include "pge/stateman.hpp"
#include "pge/text.hpp"
#include "pge/types.hpp"

class MenuClass;

class State_Menu : public pge::State {
    int font_size = 0;
    float AxisXR = 0, AxisYR = 0;
    float AxisXL = 0, AxisYL = 0;
    float TrigL2 = 0, TrigR2 = 0;
    bool TrigL1 = 0, TrigR1 = 0;
    bool DPadU = 0, DPadD = 0, DPadR = 0, DPadL = 0;
    bool ButtSel = 0, ButtSrt = 0, ButtA = 0, ButtB = 0, ButtY = 0, ButtX = 0, ButtGud = 0, ButtLS = 0, ButtRS = 0;

public:
    pge::text::Font
        fTiny{ "../res/font/unifont.ttf", pge::text::tiny },
        fSmall{ "../res/font/unifont.ttf", pge::text::small },
        fNormal{ "../res/font/unifont.ttf", pge::text::normal },
        fBig{ "../res/font/unifont.ttf", pge::text::big },
        fHuge{ "../res/font/unifont.ttf", pge::text::huge },
        fExtra{ "../res/font/unifont.ttf", pge::text::extra };

    State_Menu() { init(); }
    ~State_Menu() { cleanup(); }
    void init();
    void cleanup();

    void handle_events(SDL_Event _event);
    void update();
    void draw();
};
