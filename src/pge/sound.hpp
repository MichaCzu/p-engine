// Loading, managing and playing sounds and music.
#ifndef PGE_SOUND_H
#define PGE_SOUND_H

#include <SDL2/SDL_mixer.h>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <vector>

namespace pge::chunk {
uint16_t load(std::string _path);
std::string get_path(uint16_t _id);
bool free(std::string _path);
bool free(uint16_t _id);
bool is_free(uint16_t _id);
void purgeall();

int8_t set_volume(uint16_t _id, int8_t _volume);
int8_t set_volume_all(int8_t _volume);
void play(uint16_t _id, int16_t _loops = 0, int16_t _ticks = -1,
    int16_t _channel = -1, int8_t _volume = -1);
}

namespace pge::music {
uint16_t load(std::string _path);
std::string getpath(uint16_t _id);
bool free(std::string _path);
bool free(uint16_t _id);
bool is_free(uint16_t _id);
void purgeall();

int8_t set_volume(int8_t _volume = -1);
bool play(uint16_t _id, int16_t _loops = -1, uint16_t _fadems = 0);
bool stop(uint16_t _fadems = 0);
void pause();
void resume();
bool is_playing();
}

#endif // JUKEBOX_HPP
