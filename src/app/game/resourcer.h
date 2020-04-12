#ifndef RESOURCER_H
#define RESOURCER_H

#include "pge/image.hpp"
#include "pge/sound.hpp"
#include "pge/text.hpp"

#include <SDL2/SDL.h>
#include <vector>
#include <list>

class Controller;

struct HashtableElement
{
	uint64_t hash;
	std::string path;

	HashtableElement(uint32_t h, std::string p)
	{
		hash = h;
		path = p;
	}
};

struct SpriteStruct : HashtableElement
{
	pge::image::Image image;
	uint16_t users;
	bool loaded = false;

	SpriteStruct(uint32_t _hash, std::string _path, pge::image::Image _img) : HashtableElement(_hash, _path), image(_img)
	{
		users = 0;
	}
};
struct ChunkStruct : HashtableElement
{
	pge::sound::Chunk chunk;
	uint16_t users;

	ChunkStruct(uint32_t _hash, std::string _path, pge::sound::Chunk _chunk) : HashtableElement(_hash, _path), chunk(_chunk)
	{
		users = 0;
	}
};
struct FontStruct : HashtableElement
{
	pge::text::Font font;

	FontStruct(uint32_t _hash, std::string _path, pge::text::Font _font) : HashtableElement(_hash, _path), font(_font)
	{
	}
};

class Resourcer {

	std::vector<HashtableElement*> imageData;
	std::vector<HashtableElement*> chunkData;
	std::vector<HashtableElement*> fontData;
	
	pge::image::Image temp = { 1, 1 };
	std::vector<SpriteStruct*> imgLoadingQueue;

	Resourcer() {}
	//~Resourcer() {}

	Resourcer(Resourcer const&) = delete;
	void operator=(Resourcer const&) = delete;

	HashtableElement* find_hash(uint32_t hash, std::vector<HashtableElement*>& e, int l, int r);
	HashtableElement* find_hash(uint32_t hash, std::vector<HashtableElement*>& e);

	HashtableElement* insert_element(HashtableElement* el, std::vector<HashtableElement*>& e);
	static uint32_t string_to_hash(const char* word);
protected:
	friend Controller;
	void free_everything();
	void handle_loading(); // call from the main thread;

public:
	static Resourcer& instance()
	{
		static Resourcer instance;
		return instance;
	}

	pge::image::Image* load_image(std::string path);
	pge::sound::Chunk* load_sound(std::string path);
	pge::text::Font* load_font(std::string path);

	bool free_image(pge::image::Image sprite);
	bool free_image(std::string path);

	bool free_sound(pge::sound::Chunk chunk);
	bool free_sound(std::string path);

	bool free_font(pge::text::Font font);
	bool free_font(std::string path);
	
};

#endif
