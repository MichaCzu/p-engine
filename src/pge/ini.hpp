#ifndef PGE_INI_HPP
#define PGE_INI_HPP
#include <string>

namespace pge::ini {

uint8_t open(std::string path);
uint8_t save(uint8_t ID, std::string path);
uint8_t close(std::string path);
uint8_t close(uint8_t ID);

std::wstring read(uint8_t ID, std::wstring section, std::wstring parameter);
int read_int(uint8_t ID, std::wstring section, std::wstring parameter);
bool write(uint8_t ID, std::wstring section, std::wstring parameter, std::wstring value);
}

#endif //PGE_INI_HPP
