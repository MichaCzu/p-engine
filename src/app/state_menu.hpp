#include "pge/draw.hpp"
#include "pge/stateman.hpp"
#include "pge/types.hpp"

class MenuClass;

class State_Menu : public pge::State {
    int w, h;
    float alpha = 0;
    uint16_t bgr, bgg, bg1, bg2, bg3, bg4, bg5, bg6, title, playerImg;
    pge::viewport::View vp;
    int scale;
    double xx = 0, yy = 1600;
    float targ = 1;
    float fun = 0;
    float time_passed = 0, pastpassed = 0;
    uint8_t screen = 0, vscreen = 0;
    uint16_t sfx1, sfx2;
    uint16_t fontt, fontf, fonts, fontb;
    uint8_t quit;
    MenuClass* menu_active;

    int wa, ha;
    pge::Rect tileup[16][8], tilebot[16][6];
    bool tilemidt[16][2];

public:
    State_Menu() { init(); }
    ~State_Menu() { cleanup(); }
    void init();
    void cleanup();

    void handle_events(SDL_Event _event);
    void update();
    void draw();
};
