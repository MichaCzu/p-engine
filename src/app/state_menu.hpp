#include "app/menu/widget.h"
#include "pge/draw.hpp"
#include "pge/image.hpp"
#include "pge/sound.hpp"
#include "pge/stateman.hpp"
#include "pge/text.hpp"
#include "pge/types.hpp"

class MenuClass;

class State_Menu : public pge::State {

public:
    State_Menu() { init(); }
    ~State_Menu() { cleanup(); }
    void init();
    void cleanup();

    void handle_events(SDL_Event _event);
    void update();
    void draw();
};
