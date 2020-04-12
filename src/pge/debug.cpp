// Viewport managment, calculating drawing scale and offset, primitive drawing.
#ifndef DESIGNER_HPP
#define DESIGNER_HPP

#include "pge/debug.hpp"
#include "pge/constants.hpp"
#include "pge/core.hpp"
#include <ctime>
#include <fstream>
#include <iostream>

namespace pge {
	namespace debug {

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
			std::time_t t = std::time(0);   // get time now
			std::tm* now = std::localtime(&t);

			char buffer[10];
			sprintf(buffer, "%02d:%02d:%02d", now->tm_hour, now->tm_min, now->tm_sec);

			std::cout << "[" << buffer << "] "  << _str << std::endl;
			file << _str << std::endl;
		}

		void log(std::string caller, std::string _str)
		{
			std::cout << _str << std::endl;
			file << _str << std::endl;
		}

		void prompt(std::string _title, std::string _str, pge::debug::eErrorType _type, bool _log)
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
}

#endif // DESIGNER_HPP
