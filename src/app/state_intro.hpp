#include "pge/image.hpp"
#include "pge/sound.hpp"
#include "pge/stateman.hpp"

struct timer {
    uint8_t t;
    int s;
};

class State_Intro : public pge::State {

    pge::image::Image
        sLogo{ "../res/spr/pgi/logo.png" },
        sSpin{ "../res/spr/pgi/spin.png" },
        sSout{ "../res/spr/pgi/sout.png" },
        sPi{ "../res/spr/pgi/pi.png" },
        sIlum{ "../res/spr/pgi/ilum.png" };
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
