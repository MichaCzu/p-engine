#include "pge/stateman.hpp"

struct timer {
    uint8_t t;
    int s;
};

class State_Intro : public pge::State {

    int sLogo, sSpin, sSout, sPi, sIlum, mTheme;
    int w, h;
    uint8_t ePi = 1;
    int aPi;
    double rotate = 180;
    timer tPi[6] = { { 0, 0 }, { 0, 500 }, { 255, 1500 }, { 255, 4000 }, { 0, 5000 }, { 0, 5001 } };
    int counter;

public:
    State_Intro() { init(); }
    ~State_Intro() { cleanup(); }
    void init();
    void cleanup();

    void handle_events(SDL_Event _event);
    void update();
    void draw();
};
