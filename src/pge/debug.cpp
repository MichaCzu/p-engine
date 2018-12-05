// Viewport managment, calculating drawing scale and offset, primitive drawing.
#ifndef DESIGNER_HPP
#define DESIGNER_HPP

#include "pge/debug.hpp"
#include "pge/constants.hpp"
#include "pge/core.hpp"
#include <ctime>
#include <fstream>
#include <iostream>

namespace pge::debug {

static std::ofstream file;
static std::string _path;

void init(std::string _path)
{
    file.open(_path.c_str(), std::ofstream::out | std::ofstream::trunc);
}

void close()
{
    file.close();
}

void log(std::string _str)
{
    std::cout << _str << std::endl;
    file << _str << std::endl;
}

void prompt(std::string _title, std::string _str, uint16_t _type, bool _log)
{
    if (_log)
        log(_str);

    switch (_type) {
    case 0:
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
            _title.c_str(),
            _str.c_str(),
            NULL);
    case 1:
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING,
            _title.c_str(),
            _str.c_str(),
            NULL);
    case 2:
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
            _title.c_str(),
            _str.c_str(),
            NULL);
        pge::quit();
    }
}
}

#endif // DESIGNER_HPP
