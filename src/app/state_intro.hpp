#include "pge/image.hpp"
#include "pge/sound.hpp"
#include "pge/stateman.hpp"

struct timer {
    uint8_t t;
    int s;
};

class State_Intro : public pge::State {

    pge::image::Sprite
        sLogo{ "../res/spr/pgi/logo.pgs", nullptr },
        sSpin{ "../res/spr/pgi/spin.pgs", nullptr },
        sSout{ "../res/spr/pgi/sout.pgs", nullptr },
        sPi{ "../res/spr/pgi/pi.pgs", nullptr },
        sIlum{ "../res/spr/pgi/ilum.pgs", nullptr };
    pge::sound::Chunk
        mTheme{ "../res/mus/title.ogg" };
    int w, h;
    uint8_t ePi = 1;
    uint8_t aPi = 0;
    double rotate = 180;
    timer tPi[6] = { { 0, 0 }, { 0, 500 }, { 255, 1500 }, { 255, 4000 }, { 0, 5000 }, { 0, 5001 } };
    int counter = 0;

public:
    State_Intro() { init(); }
    ~State_Intro() { cleanup(); }
    void init();
    void cleanup();

    void handle_events(SDL_Event _event);
    void update();
    void draw();
};
