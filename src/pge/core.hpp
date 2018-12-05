#ifndef PGE_CORE_HPP
#define PGE_CORE_HPP

#include <SDL2/SDL.h>

namespace pge {
bool init();
bool close();

namespace state {
    void change(uint8_t _target); // changes game state
    void push(uint8_t _state); //adds new game state
    void stack(uint8_t _state); //adds new game state and switches to it
    void pop(uint8_t _state = 0); //removes top state - if it was the last one creates _state
    void drop(uint8_t _state = 0); //removes top state then creates _state and swich to it
    uint8_t get_active();
}

float set_defaultfps(float _newfps);
float get_defaultfps();
uint16_t get_time_difference();
uint16_t get_fps();
uint64_t get_time_passed();
void delay_frame_difference();
void handle_allevents();
void update();
void update_target(uint16_t _targetState);
void handle_statecalls();

void redraw();
void redraw_target(uint16_t _targetState);

bool is_running();
void quit();
}

#endif //PGE_CORE_HPP
