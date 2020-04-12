#include <SDL2/SDL.h>
#include <SDL2/SDL_gpu.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#ifdef NETWORKER_ACTIVE
#include <SDL2/SDL_net.h>
#endif

#include "pge/constants.hpp"
#include "pge/core.hpp"
#include "pge/pge.hpp"
#include "pge/stateman.hpp"


#include <iostream>
#include <stdint.h>
#include <tgmath.h>
#include <sched.h>
#include <vector>
#include <string>

namespace pge {

static std::vector<State*> states;
static bool running = false;
static uint8_t activeState = 0;
static double defaultFps;
static float defaultRps;

struct StateCall {
    uint8_t type;
    uint8_t state;
};
std::vector<StateCall> stateCalls;

bool init()
{
    SDL_version SDLvc;
    SDL_VERSION(&SDLvc);
    SDL_version SDLvl;
    SDL_GetVersion(&SDLvl);
    std::string SDL_Link = std::to_string(SDLvl.major) + "." + std::to_string(SDLvl.minor) + "." + std::to_string(SDLvl.patch);
    std::string SDL_Comp = std::to_string(SDLvc.major) + "." + std::to_string(SDLvc.minor) + "." + std::to_string(SDLvc.patch);

    SDL_IMAGE_VERSION(&SDLvc);
    const SDL_version* SDLIvl = IMG_Linked_Version();
    std::string IMG_Link = std::to_string(SDLIvl->major) + "." + std::to_string(SDLIvl->minor) + "." + std::to_string(SDLIvl->patch);
    std::string IMG_Comp = std::to_string(SDLvc.major) + "." + std::to_string(SDLvc.minor) + "." + std::to_string(SDLvc.patch);


    SDL_TTF_VERSION(&SDLvc);
    const SDL_version* SDLTvl = TTF_Linked_Version();
    std::string TTF_Link = std::to_string(SDLTvl->major) + "." + std::to_string(SDLTvl->minor) + "." + std::to_string(SDLTvl->patch);
    std::string TTF_Comp = std::to_string(SDLvc.major) + "." + std::to_string(SDLvc.minor) + "." + std::to_string(SDLvc.patch);

    SDL_MIXER_VERSION(&SDLvc);
    const SDL_version* SDLMvl = Mix_Linked_Version();
    std::string Mix_Link = std::to_string(SDLMvl->major) + "." + std::to_string(SDLMvl->minor) + "." + std::to_string(SDLMvl->patch);
    std::string Mix_Comp = std::to_string(SDLvc.major) + "." + std::to_string(SDLvc.minor) + "." + std::to_string(SDLvc.patch);

#ifdef NETWORKER_ACTIVE
    SDL_NET_VERSION(&SDLvc);
    const SDL_version* SDLNvl = SDLNet_Linked_Version();
    std::string NET_Link = std::to_string(SDLNvl->major) + "." + std::to_string(SDLNvl->minor) + "." + std::to_string(SDLNvl->patch);
    std::string NET_Comp = std::to_string(SDLvc.major) + "." + std::to_string(SDLvc.minor) + "." + std::to_string(SDLvc.patch);
#endif

    const SDL_version SDLGcl = GPU_GetCompiledVersion();
    const SDL_version SDLGvl = GPU_GetLinkedVersion();
    std::string GPU_Link = std::to_string(SDLGvl.major) + "." + std::to_string(SDLGvl.minor) + "." + std::to_string(SDLGvl.patch);
    std::string GPU_Comp = std::to_string(SDLGcl.major) + "." + std::to_string(SDLGcl.minor) + "." + std::to_string(SDLGcl.patch);

    debug::init("log.txt");
    debug::log("Initializing SDL " + SDL_Link + " (Compiled for " + SDL_Comp + ")..");
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS) < 0) {
        debug::log("SDL2 Initialization failed: " + (std::string)SDL_GetError());
        return false;
    }

    debug::log("Initializing SDL_image " + IMG_Link + " (Compiled for " + IMG_Comp + ")..");
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        debug::log("SDL_image Initialization failed: " + (std::string)IMG_GetError());
        debug::log(IMG_GetError());
        return false;
    }


    // SDL_SetWindowIcon( window, sur_icon );
    debug::log("Initializing SDL_TTF.. " + TTF_Link + " (Compiled for " + TTF_Comp + ")..");
    if (TTF_Init() == -1) {
        debug::log("SDL_TTF Initialization failed: " + (std::string)TTF_GetError());
    }

    debug::log("Initializing SDL_mixer.. " + Mix_Link + " (Compiled for " + Mix_Comp + ")..");
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        debug::log("SDL_mixer opening audio failed: " + (std::string)Mix_GetError());
        return false;
    }
    if (!(Mix_Init(MIX_INIT_OGG) & MIX_INIT_OGG)) {
        debug::log("SDL_mixer Initialization failed: " + (std::string)Mix_GetError());
        return false;
    }

#ifdef NETWORKER_ACTIVE
    debug::log("Initializing SDL_net.. " + NET_Link + " (Compiled for " + NET_Comp + ")..");
    if (SDLNet_Init()) {
        debug::log("SDL_net Initialization failed: " + (std::string)SDLNet_GetError());
        return false;
    }
#endif

    SDL_JoystickEventState(SDL_ENABLE);
    SDL_GameControllerEventState(SDL_ENABLE);
    debug::log("Using SDL_GPU " + GPU_Link + " (Compiled for " + GPU_Comp + ")");
    debug::log("Creating new window..");
    window::create(DEFAULT_WIN_WIDTH, DEFAULT_WIN_HEIGHT);

//window::rename(DEFAULT_WIN_TITLE);

#ifdef SHADERS_ON
    pge::shader::init();
#endif

    locale::resort();
    debug::log("Done!");
    set_defaultfps(DEFAULT_FRAMERATE);
    running = true;
    return true;
}

bool close()
{
    debug::close();
    sound::purgeall();
    music::purgeall();
    image::purgeall();
    //text::purgeall();

    for (uint8_t i = states.size(); i > 0; i--) {
        state::pop();
    }
    handle_statecalls();

    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    GPU_Quit();
    SDL_Quit();
    return true;
}

namespace state {
    void change(uint8_t _target)
    {
        StateCall flag = { 0, _target };
        stateCalls.push_back(flag);
    }

    void push(uint8_t _state)
    {
        StateCall flag = { 1, _state };
        stateCalls.push_back(flag);
    }

    void stack(uint8_t _state)
    {
        StateCall flag = { 2, _state };
        stateCalls.push_back(flag);
    }

    void pop(uint8_t _state)
    {
        StateCall flag = { 3, _state };
        stateCalls.push_back(flag);
    }

    void drop(uint8_t _state)
    {
        StateCall flag = { 4, _state };
        stateCalls.push_back(flag);
    }

    uint8_t get_active()
    {
        return activeState;
    }
}

void handle_statecalls()
{
    if (!stateCalls.empty()) {
        for (uint8_t i = 0; i < stateCalls.size(); i++) {
            switch (stateCalls[i].type) {
            case 0: //change_state
                activeState = stateCalls[i].state;
                break;
            case 1: //push_state
                if (states.size() < 16) {
                    states.push_back(get_state(stateCalls[i].state));
                }
                break;
            case 2: //stack_state
                if (states.size() < 16) {
                    states.push_back(get_state(stateCalls[i].state));
                    if (states.size() > 1) {
                        activeState = states.size() - 1;
                    }
                }
                break;
            case 3: //pop_state
                if (states.size() > 0) {
                    delete states[states.size() - 1];
                    states.pop_back();
                }
                if (states.size() == 0) {
                    if (stateCalls[i].state > 0)
                        state::push(stateCalls[i].state);
                    else
                        quit();
                }
                break;
            case 4: //drop_state
                if (states.size() > 0) {
                    delete states[states.size() - 1];
                    states.pop_back();
                    if (activeState > 0 && states.size() > 0)
                        activeState--;
                }
                if (states.size() == 0) {
                    if (stateCalls[i].state > 0)
                        state::push(stateCalls[i].state);
                    else
                        quit();
                }
                break;
            }
        }
        stateCalls.clear();
    }
}

//

static const uint8_t _maxSamples = 255;
static float _lastTick = 0, _diffTick[_maxSamples] = { 0 }, _lastDiff = 0, _Ffps = 0, _tickSum = 0;
static uint8_t selector;

float get_defaultRps()
{
	return defaultRps;
}

float get_defaultFps()
{
	return defaultFps;
}

float set_defaultfps(float _newfps)
{
    if (_newfps < 1) {
        defaultRps = 0;
        defaultFps = 0;
        return 0;
    }
    float buffer = 1000 / _newfps;
    buffer = modf(buffer, &defaultFps);
	defaultFps = 1000 / _newfps;
	defaultRps = buffer;
    return 1000 / defaultFps;
}

float get_defaultfps()
{
    if (defaultFps + defaultRps > 0)
        return 1000 / (defaultFps + defaultRps);
    return 0;
}

uint16_t get_time_difference()
{
    return _lastDiff;
}

uint16_t get_fps()
{
	return _Ffps;
}

void delay_frame_difference()
{
    static float sum = 0;
    static int nextTick = 0;

    while (nextTick > SDL_GetTicks()) {
		if (nextTick - SDL_GetTicks() > 1)
			SDL_Delay(1);
    }
    uint32_t ticks = SDL_GetTicks();
    sum += defaultRps;

    if (sum >= 1) {
        nextTick = ticks + defaultFps + 1;
        sum--;
    } else
        nextTick = ticks + defaultFps;
	SDL_Delay(1);

	_tickSum -= _diffTick[selector];
	_lastDiff = ticks - _lastTick;

    _diffTick[selector] = _lastDiff;
	_tickSum += _lastDiff;
    _lastTick = ticks;

    selector++;
    if (selector >= _maxSamples)
        selector = 0;
	_Ffps = SDL_ceil(1000.f / SDL_max(_tickSum / (float)_maxSamples, 0.1f));
}

uint64_t get_time_passed()
{
    return SDL_GetTicks();
}


bool sizeMoveTimerRunning = false;
void handle_allevents()
{
    static SDL_Event _event;
    while (SDL_PollEvent(&_event)) {
        pge::input::handle_input_event(_event);
        if (_event.type == SDL_CONTROLLERDEVICEADDED) {
            SDL_GameController* controller = NULL;
            if (SDL_IsGameController(_event.cdevice.which))
                controller = SDL_GameControllerOpen(_event.cdevice.which);
            if (controller)
                pge::debug::log("gamecontroller opened");
            else
                pge::debug::log("Could not open gamecontroller " + std::to_string(_event.cdevice.which) + ": " + SDL_GetError());
            continue;
        } else if (_event.type == SDL_CONTROLLERDEVICEREMOVED) {
            SDL_GameControllerClose(SDL_GameControllerFromInstanceID(_event.cdevice.which));
            continue;
        }
        if (_event.type == SDL_MOUSEMOTION) {
            continue;
        }
        if (_event.type == SDL_WINDOWEVENT) {
            switch (_event.window.event) {
            case SDL_WINDOWEVENT_SHOWN:
            case SDL_WINDOWEVENT_SIZE_CHANGED:
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_MAXIMIZED:
                //pge::debug::log("Window resized to " + std::to_string(_event.window.data1) + "x" + std::to_string(_event.window.data2));
                pge::window::set_resolution(_event.window.data1, _event.window.data2);
                continue;
            }
        } else if (_event.type == SDL_QUIT) {
            quit();
            continue;
        } else if (_event.key.type == SDL_KEYDOWN) {
            if (_event.key.keysym.sym == SDLK_F11 && _event.key.repeat == 0 && _event.key.state == SDL_PRESSED) {
                pge::window::set_fullscreen(!pge::window::is_fullscreen());
                continue;
            }
        }
        states[activeState]->handle_events(_event);
    }
}
void update()
{
    states[activeState]->update();
}

void update_target(uint16_t _targetState)
{
    states[_targetState]->update();
}

void redraw()
{
    states[activeState]->draw();
}

void redraw_target(uint16_t targetState)
{
    states[targetState]->draw();
}

bool is_running()
{
    return running;
}

void quit()
{
    running = false;
}
}
