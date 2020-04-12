//zapisywanie i wczytywanie danych z pliku. ( niby zbędne ale dla czytelności potem )
#pragma once
#include <string>
#include <vector>

namespace pge {
	namespace file {
bool init();
bool close();
bool remove(std::string path);

std::wstring read_nextline(std::string path);
std::wstring read(std::string path, int mode = 0);
bool write(std::string path, std::wstring text, int mode = 0);

std::vector<std::string> get_folderlist(std::string path);
}
}