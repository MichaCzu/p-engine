// Viewport managment, calculating drawing scale and offset, primitive drawing.
#ifndef PGE_DEBUG_HPP
#define PGE_DEBUG_HPP

#include "pge/constants.hpp"
#include <SDL2/SDL.h>
#include <stdint.h>

namespace pge {

namespace debug {

    void init(std::string _path);
    void close();

    void log(std::string _str);
    void prompt(std::string _title, std::string _str, uint16_t _type, bool _log = true);
    //2 - critical
    //1 - warning
    //0 - info
}
}
#endif // DEBUGGER_HPP
