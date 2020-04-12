#include "pge/sound.hpp"
#include "pge/debug.hpp"
#include <SDL2/SDL_mixer.h>

uint8_t defvol = 128;

namespace pge::sound {
struct StructChunk {
    Mix_Chunk* chunk;
    int8_t volume;
    std::string path;
};
static std::vector<StructChunk*> vChunk;
static uint16_t find_first_free_chunk();

// TODO: Search for copy first
uint16_t load(std::string _path)
{
    Mix_Chunk* loadChunk = Mix_LoadWAV(_path.c_str());
    uint16_t _id;

    debug::log("Loading " + _path);
    if (!loadChunk) {
        debug::log("Couldn't load: " + _path + " - " + Mix_GetError());
        debug::log("Aborting...");
        return (uint16_t)-1;
    } else {
        _id = find_first_free_chunk();
        if (_id == (uint16_t)-1) {
            vChunk.push_back(new StructChunk);
            _id = vChunk.size() - 1;
        }
        vChunk[_id]->chunk = loadChunk;
        vChunk[_id]->path = _path;
        vChunk[_id]->volume = 127;
        return _id;
    }
}

std::string get_path(uint16_t _id) { return vChunk[_id]->path; }

bool free(std::string _path)
{
    for (int i = 0; i < vChunk.size(); i++) {
        if (vChunk[i]->path == _path) {
            sound::free(i);
            return true;
        }
    }
    return false;
}

bool free(uint16_t _id)
{
    if (vChunk.size() > _id || vChunk[_id]->chunk != NULL) {
        Mix_FreeChunk(vChunk[_id]->chunk);
        vChunk[_id]->chunk = NULL;
        vChunk[_id]->path = "";
        return true;
    } else
        return false;
}

bool is_free(uint16_t _id)
{
    if (vChunk.size() > _id || vChunk[_id]->chunk != NULL)
        return true;
    else
        return false;
}

void purgeall()
{
    for (int i = vChunk.size() - 1; i >= 0; i--) {
        sound::free((uint16_t)i);
        vChunk.pop_back();
    }
}

int8_t set_volume(uint16_t _id, int8_t _volume)
{
    vChunk[_id]->volume = _volume;
    return Mix_VolumeChunk(vChunk[_id]->chunk, _volume);
}

int8_t set_volume_all(int8_t _volume)
{
    int8_t flag = defvol;
    defvol = _volume;
    //for (int i = 0; i < vChunk.size(); i++)
    //    set_volume(i, _volume);
    return flag;
}

void play(uint16_t _id, int16_t _loops, int16_t _ticks,
    int16_t _channel, int8_t _volume)
{
    if (_volume >= 0) {
        Mix_VolumeChunk(vChunk[_id]->chunk, _volume * defvol / 127);
    } else
        Mix_VolumeChunk(vChunk[_id]->chunk, vChunk[_id]->volume * defvol / 127);

    Mix_PlayChannelTimed(0, vChunk[_id]->chunk, _loops, _ticks);
}

static uint16_t find_first_free_chunk()
{
    for (int i = 0; i < vChunk.size(); i++) {
        if (vChunk[i]->chunk == NULL) {
            return i;
        }
    }
    return (uint16_t)-1;
}
}

namespace pge::music {
struct StructMusic {
    Mix_Music* music;
    std::string path;
};
static uint16_t find_first_free_music();
static std::vector<StructMusic*> vMusic;
//TODO: Search for copy first
uint16_t load(std::string _path)
{
    Mix_Music* loadMusic = Mix_LoadMUS(_path.c_str());
    uint16_t _id;
    debug::log("Loading " + _path);
    if (!loadMusic) {
        debug::log("Couldn't load: " + _path + " - " + Mix_GetError());
        debug::log("Aborting...");
        return (uint16_t)-1;
    } else {
        _id = find_first_free_music();
        if (_id == (uint16_t)-1) {
            vMusic.push_back(new StructMusic);
            _id = vMusic.size() - 1;
        }
        vMusic[_id]->music = loadMusic;
        vMusic[_id]->path = _path;
        return _id;
    }
}

std::string get_path(uint16_t _id) { return vMusic[_id]->path; }

bool free(std::string _path)
{
    for (int i = 0; i < vMusic.size(); i++) {
        if (vMusic[i]->path == _path) {
            music::free(i);
            return true;
        }
    }
    return false;
}

bool free(uint16_t _id)
{
    if (vMusic.size() > _id || vMusic[_id]->music != NULL) {
        Mix_FreeMusic(vMusic[_id]->music);
        vMusic[_id]->music = NULL;
        vMusic[_id]->path = "";
        return true;
    } else
        return false;
}

bool is_free(uint16_t _id)
{
    if (vMusic.size() > _id || vMusic[_id]->music != NULL)
        return true;
    else
        return false;
}

void purgeall()
{
    for (int i = vMusic.size() - 1; i >= 0; i--) {
        music::free((uint16_t)i);
        vMusic.pop_back();
    }
}

int8_t set_volume(int8_t _volume)
{
    return Mix_VolumeMusic(_volume);
}

bool play(uint16_t _id, int16_t _loops, uint16_t _fadems)
{
    if (_fadems == 0)
        return Mix_PlayMusic(vMusic[_id]->music, _loops);
    else
        return Mix_FadeInMusic(vMusic[_id]->music, _loops, _fadems);
}

bool stop(uint16_t _fadems)
{
    if (_fadems > 0)
        return Mix_FadeOutMusic(_fadems);
    else
        return Mix_HaltMusic();
}
void pause() { Mix_PauseMusic(); }
void resume() { Mix_ResumeMusic(); }
bool is_playing() { return Mix_PlayingMusic(); }

static uint16_t find_first_free_music()
{
    for (int i = 0; i < vMusic.size(); i++) {
        if (vMusic[i]->music == NULL) {
            return i;
        }
    }
    return (uint16_t)-1;
}
}
