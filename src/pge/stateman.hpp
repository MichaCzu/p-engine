// Game state template.
#ifndef PGE_STATEMAN_HPP
#define PGE_STATEMAN_HPP

#include <SDL2/SDL.h>

namespace pge {

class State {
public:
    virtual ~State() {}
    virtual void init() = 0;
    virtual void cleanup() = 0;

    virtual void handle_events(SDL_Event _event) = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
};
}

#endif // STATEMAN_HPP
