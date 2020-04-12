#include "resourcer.h"
#include "pge/debug.hpp"
const int tablesize = (uint32_t)(-1);

pge::image::Image* Resourcer::load_image(std::string path)
{
	uint32_t hash = string_to_hash(path.c_str());
	SpriteStruct* returnData = (SpriteStruct*)(find_hash(hash, imageData));
	if (returnData == nullptr)
	{
		//if (img == nullptr)
		//	return nullptr;
		SpriteStruct* newElement = new SpriteStruct(hash, path, temp);
		returnData = (SpriteStruct*)insert_element(newElement, imageData);
		imgLoadingQueue.push_back(returnData);
	}

	//pge::debug::log(path+": "+std::to_string(hash));
	returnData->users++;
	return &returnData->image;
	//return nullptr;
}

pge::sound::Chunk* Resourcer::load_sound(std::string path)
{
	return nullptr;
}

pge::text::Font* Resourcer::load_font(std::string path)
{
	return nullptr;
}

bool Resourcer::free_image(pge::image::Image sprite)
{
	return false;
}

bool Resourcer::free_image(std::string path)
{
	return false;
}

bool Resourcer::free_sound(pge::sound::Chunk chunk)
{
	return false;
}

bool Resourcer::free_sound(std::string path)
{
	return false;
}

bool free_font(pge::text::Font font)
{
	return false;
}

bool Resourcer::free_font(std::string path)
{
	return false;
}

HashtableElement* Resourcer::find_hash(uint32_t hash, std::vector<HashtableElement*>& e)
{
	return find_hash(hash, e, 0, e.size()-1);
}

HashtableElement* Resourcer::find_hash(uint32_t hash, std::vector<HashtableElement*>& e, int l, int r)
{
	if (r >= l) {
		int mid = l + (r - l) / 2;

		if (e[mid]->hash == hash)
			return e[mid];

		if (e[mid]->hash > hash)
			return find_hash(hash, e, l, mid-1);


		return find_hash(hash, e, mid + 1, r);
	}
	return nullptr;
}

HashtableElement* Resourcer::insert_element(HashtableElement* el, std::vector<HashtableElement*>& e)
{
	int a = 0;
	int b = e.size() - 1;

	while(a < b) {
		int m = a + (b - a) / 2;

		// Check if x is present at mid 
		if (e[m]->hash == el->hash)
		{
			//pge::debug::prompt("Error", "This shouldn't have happened", pge::debug::critical, true);
			return nullptr;
		}

		// If x greater, ignore left half 
		if (e[m]->hash < el->hash)
			a = m + 1;

		// If x is smaller, ignore right half 
		else
			b = m - 1;
	}

	e.insert(e.begin()+a, el);
	//pge::debug::log(std::to_string(e.size()));
	return e[a];
}

uint32_t Resourcer::string_to_hash(const char *word)
{
	const uint16_t prime = 7723;
	const uint16_t prime2 = 9973;
	uint32_t hashAddress = 0;
	while(*word) {
		hashAddress = (hashAddress * prime) ^ (word[0] * prime2);
		word++;
	}
	return hashAddress;
}

void Resourcer::free_everything()
{
	for (int i = 0; i < imageData.size(); i++)
	{
		((SpriteStruct*)(imageData[i]))->image._free();
		delete imageData[i];
	}
	imageData.erase(imageData.begin(), imageData.end());
	/*
	for (int i = 0; i <= chunkData.size(); i++)
	{
		((SpriteStruct*)(chunkData[i]))->image._free();
		delete chunkData[i];
	}
	chunkData.erase(chunkData.begin(), chunkData.end());

	for (int i = 0; i <= fontData.size(); i++)
	{
		((SpriteStruct*)(fontData[i]))->image._free();
		delete fontData[i];
	}
	fontData.erase(fontData.begin(), fontData.end());
	*/
}

void Resourcer::handle_loading()
{
	if (imgLoadingQueue.size() == 0)
		return;

	int range = imgLoadingQueue.size();
	for (int i = 0; i < range; i++)
	{
		pge::image::Image img = { imgLoadingQueue[i]->path };
		imgLoadingQueue[i]->image = img;
	}
	imgLoadingQueue.erase(imgLoadingQueue.begin(), imgLoadingQueue.begin()+range);
}