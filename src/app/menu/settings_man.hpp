#ifndef SETTINGS_MAN
#define SETTINGS_MAN

#include "pge/pge.hpp"

bool settings_load();
bool settings_save();

void settings_set_soundvolume(uint8_t);
uint8_t settings_get_soundvolume();
void settings_set_musicvolume(uint8_t);
uint8_t settings_get_musicvolume();

void settings_set_language(std::string);
std::string settings_get_language();
std::vector<std::string> settings_get_languagelist();

void settings_set_quality(uint8_t);
uint8_t settings_get_quality();
void settings_set_fullscreen(bool);
void settings_set_fps(uint16_t);

void settings_set_controls();
void settings_get_controls();

#endif
