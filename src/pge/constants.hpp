// Important variables for quick engine configuration
#pragma once

#include <SDL2/SDL.h>
#include <stdint.h>
#include <string>

namespace pge {

class State;

enum en_states { es_intro = 1,
    es_menu = 2,
    es_game = 3 };

State* get_state(uint8_t id);

//#define SHADERS_ON
const std::string ENGINE_VERSION = "1.0.0";
const std::wstring GAME_VERSION = L"prealpha";
const char DEFAULT_WIN_TITLE[] = "The Shattered Lands";
const char CRYPTER_KEY[] = "THELONGESTFUCKINGCRYPTINGKEYINTHEFUCKINGWORLD";
const uint16_t DEFAULT_WIN_WIDTH = 720;
const uint16_t DEFAULT_WIN_HEIGHT = 480;
const uint16_t DEFAULT_WIN_MINWIDTH = 640;
const uint16_t DEFAULT_WIN_MINHEIGHT = 480;
const uint16_t DEFAULT_WIN_MAXWIDTH = 0;
const uint16_t DEFAULT_WIN_MAXHEIGHT = 0;
const float DEFAULT_FRAMERATE = 240; // set 0 for infinity
const uint8_t DEFAULT_FULLSCREEN = 1;
const uint8_t DEFAULT_SCALE_ACCURACY = 0; // 0 - x; 1 - 0.x; 2 - 0.0x; 3 -
// 0.000x
#define DEV_MODE
//#define NETWORKER_ACTIVE
const bool DEFAULT_DEBUG = false;
const std::string DEFAULT_LOGFILE = "log.txt";
}