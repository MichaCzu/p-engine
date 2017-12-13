#include "pge/crypt.hpp"
#include "pge/constants.hpp"
#include "pge/file.hpp"
#include <fstream>

namespace pge::crypt {

std::string string(std::string _string, char* key)
{
    if (key == NULL)
        key = (char*)CRYPTER_KEY;
    std::string output = _string;

    for (int i = 0; i < _string.size(); i++)
        output[i] = _string[i] ^ key[i % (sizeof(key) / sizeof(char))];

    return output;
}

std::wstring string(std::wstring _string, char* key)
{
    if (key == NULL)
        key = (char*)CRYPTER_KEY;
    std::wstring output = _string;

    for (int i = 0; i < _string.size(); i++)
        output[i] = _string[i] ^ key[i % (sizeof(key) / sizeof(char))];

    return output;
}

bool file(std::string path, char* key)
{
    std::string buffer = "";
    std::fstream fstr(path, std::ios::binary | std::ios::in);
    pge::file::close();

    if (fstr.is_open()) {
        for (char c; fstr.get(c);) {
            buffer += c;
        }
        buffer = string(buffer, key);
        fstr.close();
        fstr.clear();
        fstr.open(path, std::ios::binary | std::ios::out | std::ios::trunc);
        for (uint i = 0; i < buffer.length(); i++) {
            fstr.put(buffer[i]);
        }
    }
    return 1;
}

bool file_ex(std::string pathi, std::string patho, char* key)
{
    std::string buffer = "";
    std::fstream fstr(pathi, std::ios::binary | std::ios::in);
    pge::file::close();

    if (fstr.is_open()) {
        for (char c; fstr.get(c);) {
            buffer += c;
        }
        buffer = string(buffer, key);
        fstr.close();
        fstr.clear();

        fstr.open(patho, std::ios::binary | std::ios::out | std::ios::trunc);
        for (uint i = 0; i < buffer.length(); i++) {
            fstr.put(buffer[i]);
        }
        fstr.close();
        fstr.clear();
    }
    return 1;
}
}

namespace pge::random {
bool set_seed(uint32_t seed)
{
    if (seed == (uint32_t)(-1))
        srand(time(NULL));
    else
        srand(seed);

    return 0;
}

int32_t get(int32_t min, int32_t max) { return rand() % (max + 1 - min) + min; }
}
