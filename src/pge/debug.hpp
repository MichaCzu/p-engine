// Viewport managment, calculating drawing scale and offset, primitive drawing.
#pragma once

#include "pge/constants.hpp"
#include <SDL2/SDL.h>
#include <stdint.h>

namespace pge {
	namespace debug {
		enum eErrorType
		{
			info = 0,
			warning = 1,
			critical = 2
		};

    void init(std::string _path);
    void close();

    void log(std::string _str);
	void log(std::string caller, std::string _str);
    void prompt(std::string _title, std::string _str, pge::debug::eErrorType _type, bool _log = true);
    //2 - critical
    //1 - warning
    //0 - info
	}
}
