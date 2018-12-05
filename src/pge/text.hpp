//zarządzanie czcionkami i wyświetlanie tekstu zgodnie z viewportem(jeśli trzeba) i skalą.
#ifndef PGE_TEXT_HPP
#define PGE_TEXT_HPP

#include <SDL2/SDL_ttf.h>
#include <string>
namespace pge {
namespace text {

    enum size : uint16_t {
        tiny = 8,
        small = 13,
        normal = 16,
        big = 20,
        huge = 26,
        extra = 31
    };
    //8
    //13
    //16
    //20
    //25
    //28
    //31

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

    class Font {
        int id;

    public:
        Font(std::string _path, int _ptsize) { id = text::load(_path, _ptsize); }
        ~Font() { text::free(id); }

        bool resize(int _ptsize) { return text::resize(id, _ptsize); }
        std::string get_path() { return text::get_path(id); }
        bool draw(int x, int y, std::wstring text) { return text::draw(id, x, y, text); }
        bool draw(int x, int y, std::wstring text, SDL_Color color) { return text::draw(id, x, y, text, color); }
        bool drawMid(int x, int y, std::wstring text) { return text::draw(id, x - text::get_width(id, text) / 2, y, text); }
        bool drawMid(int x, int y, std::wstring text, SDL_Color color) { return text::draw(id, x - text::get_width(id, text) / 2, y, text, color); }

        uint16_t get_width(std::wstring text) { return text::get_width(id, text); }
        uint16_t get_height(std::wstring text) { return text::get_height(id, text); }
    };
}
}

#endif //WRITER_HPP
