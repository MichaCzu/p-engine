#pragma once
#include <string>

namespace pge {
	namespace ini {

uint8_t open(std::string path);
uint8_t save(uint8_t ID, std::string path);
uint8_t close(std::string path);
uint8_t close(uint8_t ID);

std::wstring read(uint8_t ID, std::wstring section, std::wstring parameter);
int read_int(uint8_t ID, std::wstring section, std::wstring parameter);
bool write(uint8_t ID, std::wstring section, std::wstring parameter, std::wstring value);
}
}