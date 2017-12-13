//zarządzanie czcionkami i wyświetlanie tekstu zgodnie z viewportem(jeśli trzeba) i skalą.
#ifndef PGE_TEXT_HPP
#define PGE_TEXT_HPP

#include <SDL2/SDL_ttf.h>
#include <string>

namespace pge::text {
uint16_t load(std::string _path, int _ptsize);
bool resize(uint16_t _id, int _ptsize);
std::string get_path(uint16_t _id);
bool free(std::string _path);
bool free(uint16_t _id);
bool is_free(uint16_t _id);
void purgeall();

bool draw(int _f, int x, int y, std::wstring text);
bool draw(int _f, int x, int y, std::wstring text, SDL_Color color);

uint16_t get_width(int _f, std::wstring text);
uint16_t get_height(int _f, std::wstring text);

std::wstring get_cut(int _f, std::wstring text, int width);
}

#endif //WRITER_HPPh
