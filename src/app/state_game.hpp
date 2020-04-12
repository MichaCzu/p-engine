#include "pge/stateman.hpp"
#include <string>
class Controller;

class State_Game : public pge::State {
    int w, h;
    float alpha;
    int fontf;
    Controller* ctrl;

public:
    State_Game() { init(); }
    ~State_Game() { cleanup(); }
    void init();
    void cleanup();

    void handle_events(SDL_Event _event);
    void update();
    void draw();
};
