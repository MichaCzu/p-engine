#include "pge/text.hpp"
#include "pge/constants.hpp"
#include "pge/debug.hpp"
#include "pge/draw.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_gpu.h>
#include <SDL2/SDL_ttf.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string.h>

struct StructGlyph {
    uint16_t ID;
    GPU_Image* glyph;
    int w;
    int h;
    int x;
    int xx;
    int y;
    int yy;
    int ad;
};

struct StructFont {
    TTF_Font* font;
    std::vector<StructGlyph*> glyph;
    std::string path;
    int h;
    int a;
    int d;
    int ls;
};

static std::vector<StructFont*> vFont;
static uint16_t find_first_free_font();

namespace pge {
	namespace text {
		//static uint16_t free();

		uint16_t load(std::string _path, int _ptsize)
		{
			TTF_Font* loadFont = TTF_OpenFont(_path.c_str(), _ptsize);
			uint16_t _id;

			debug::log("Loading " + _path);
			if (!loadFont) {
				debug::log("Couldn't load " + _path);
				debug::log("Aborting...");
				return (uint16_t)-1;
			}
			else {
				_id = find_first_free_font();
				if (_id == (uint16_t)-1) {
					vFont.push_back(new StructFont);
					_id = vFont.size() - 1;
				}
				vFont[_id]->font = loadFont;
				vFont[_id]->path = _path;
				vFont[_id]->a = TTF_FontAscent(loadFont);
				vFont[_id]->d = TTF_FontDescent(loadFont);
				vFont[_id]->h = TTF_FontHeight(loadFont);
				vFont[_id]->ls = TTF_FontLineSkip(loadFont);
				return _id;
			}
		}

		bool resize(uint16_t _f, int _ptsize)
		{
			if (vFont[_f]->h != _ptsize) {
				if (text::is_free(_f)) {
					for (int i = 0; i < vFont[_f]->glyph.size(); i++) {
						GPU_FreeImage(vFont[_f]->glyph[i]->glyph);
					}
					vFont[_f]->glyph.clear();
					TTF_CloseFont(vFont[_f]->font);
					vFont[_f]->font = TTF_OpenFont(vFont[_f]->path.c_str(), _ptsize);
					vFont[_f]->a = TTF_FontAscent(vFont[_f]->font);
					vFont[_f]->d = TTF_FontDescent(vFont[_f]->font);
					vFont[_f]->h = TTF_FontHeight(vFont[_f]->font);
					vFont[_f]->ls = TTF_FontLineSkip(vFont[_f]->font);
					return true;
				}
				else {
					return false;
				}
			}
			return true;
		}

		std::string get_path(uint16_t _id) { return vFont[_id]->path; }

		bool free(std::string _path)
		{
			for (int i = 0; i < vFont.size(); i++) {
				if (vFont[i]->path == _path) {
					text::free(i);
					return true;
				}
			}
			return false;
		}

		bool free(uint16_t _id)
		{
			if (vFont.size() > _id || vFont[_id]->font != NULL) {
				for (int i = 0; i < vFont[_id]->glyph.size(); i++) {
					GPU_FreeImage(vFont[_id]->glyph[i]->glyph);
				}
				vFont[_id]->glyph.clear();
				TTF_CloseFont(vFont[_id]->font);
				vFont[_id]->font = NULL;
				vFont[_id]->path = "";
				return true;
			}
			else
				return false;
		}

		bool is_free(uint16_t _id)
		{
			if (vFont.size() > _id || vFont[_id]->font != NULL)
				return true;
			else
				return false;
		}

		void purgeall()
		{
			for (int i = vFont.size() - 1; i >= 0; i--) {
				text::free((uint16_t)i);
				vFont.pop_back();
			}
		}

		static StructGlyph* create_glyph(uint16_t _font, Uint16 letter)
		{
			StructGlyph* flag = new StructGlyph;
			SDL_Color color = { 255, 255, 255 };
			SDL_Surface* surface;

			if (!(surface = TTF_RenderGlyph_Solid(vFont[_font]->font, letter, color))) {
				return NULL;
			}
			else {
				flag->glyph = GPU_CopyImageFromSurface(surface);
				//int x, xx, y, yy, hh, ad;
				TTF_GlyphMetrics(vFont[_font]->font, letter, &flag->x, &flag->xx, &flag->y, &flag->yy, &flag->ad);
				flag->ID = letter;
				flag->w = surface->w;
				flag->h = surface->h;
				SDL_FreeSurface(surface);
				return flag;
			}
		}

		static uint16_t get_glyph(uint16_t _font, Uint16 letter)
		{
			int start = 0;
			int end = vFont[_font]->glyph.size() - 1;
			int mid = 0;

			if (vFont[_font]->glyph.size() > 0) {
				while (start <= end) {
					mid = (start + end) / 2;
					if (letter == vFont[_font]->glyph[mid]->ID)
						return mid;
					else if (letter < vFont[_font]->glyph[mid]->ID)
						end = mid - 1;
					else
						start = mid + 1;
				}
			}
			StructGlyph* flag = create_glyph(_font, letter);
			if (flag)
				vFont[_font]->glyph.insert(vFont[_font]->glyph.begin() + mid, flag);
			else
				mid = get_glyph(_font, L'�');
			return mid;
		}

		bool draw(int _f, int x, int y, std::wstring text)
		{
			int a = 0;
			int ns = 0;
			for (int i = 0; i < text.length(); i++) {
				Uint16 cha = text[i];
				if (text[i] == L'\n') {
					a = 0;
					ns += vFont[_f]->ls;
				}
				else {
					uint16_t _fglyph = get_glyph(_f, cha);
					StructGlyph* _glyph = vFont[_f]->glyph[_fglyph];
					GPU_Rect test = { float(x + a), float(y + ns), (float)_glyph->w, (float)_glyph->h };
					a += _glyph->ad;
					//uint8_t _r, _g, _b, _a;
					//pge::rgb_to_vars(color, &_r, &_g, &_b, &_a);
					//SDL_SetTextureAlphaMod(_glyph->glyph, _a);
					//SDL_SetTextureColorMod(_glyph->glyph, _r, _g, _b);
					GPU_BlitRect(_glyph->glyph, NULL, window::get_renderer(), &test);
				}
			}
			return true;
		}

		bool draw(int _f, int x, int y, std::wstring text, SDL_Color color)
		{
			int a = 0;
			int ns = 0;
			for (int i = 0; i < text.length(); i++) {
				Uint16 cha = text[i];
				if (text[i] == L'\n') {
					a = 0;
					ns += vFont[_f]->ls;
				}
				else {
					uint16_t _fglyph = get_glyph(_f, cha);
					StructGlyph* _glyph = vFont[_f]->glyph[_fglyph];
					GPU_Rect test = { float(x + a), float(y + ns), (float)_glyph->w, (float)_glyph->h };
					a += _glyph->ad;

					GPU_SetColor(_glyph->glyph, color);
					GPU_BlitRect(_glyph->glyph, NULL, window::get_renderer(), &test);
				}
			}
			return true;
		}

		uint16_t get_width(int _f, std::wstring text)
		{
			int buffer = 0, rebuffer = 0;
			for (int i = 0; i < text.length(); i++) {
				if (text[i] == L'\n') {
					if (buffer > rebuffer)
						rebuffer = buffer;
					buffer = 0;
				}
				else {
					uint16_t _fglyph = get_glyph(_f, text[i]);
					buffer += vFont[_f]->glyph[_fglyph]->w;
				}
			}
			return std::max(rebuffer, buffer);
		}

		std::wstring get_cut(int _f, std::wstring text, int _length)
		{
			std::wstring buffer = L"", flag = L"", word = L"";
			uint16_t wwidth = 0, fwidth = 0;

			uint16_t _fglyph = get_glyph(_f, L' ');
			int ws = vFont[_f]->glyph[_fglyph]->w;

			for (int i = 0; i < text.length(); i++) {
				if ((fwidth + wwidth + ws > _length)) {
					buffer += flag + L'\n';
					flag.clear();
					flag = word;
					word.clear();
					fwidth = wwidth + ws;
					wwidth = 0;
					i--;
				}
				else if (text[i] == L'\n') {
					buffer += flag + word + L'\n';
					flag.clear();
					word.clear();
					fwidth = 0;
					wwidth = 0;
				}
				else if (text[i] == L' ') {
					flag += word + L" ";
					word.clear();
					fwidth += wwidth + ws;
					wwidth = 0;
				}
				else {
					word += text[i];
					uint16_t _fglyph = get_glyph(_f, text[i]);
					wwidth += vFont[_f]->glyph[_fglyph]->w;
				}
			}
			return buffer + flag + word;
		}

		uint16_t get_height(int _f, std::wstring text)
		{
			int a = 0;
			for (int i = 0; i < text.length(); i++) {
				if (text[i] == L'\n') {
					a++;
				}
			}
			return vFont[_f]->h + vFont[_f]->ls * a;
		}
		//

	}
}

static uint16_t find_first_free_font()
{
    for (int i = 0; i < vFont.size(); i++) {
        if (vFont[i]->font == NULL) {
            return i;
        }
    }
    return (uint16_t)-1;
}
