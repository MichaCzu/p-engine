//szyfrowanie stringów, plików i całych archiwów.
#pragma once

#include "pge/constants.hpp"
#include <stdlib.h>
#include <string>
#include <time.h>

namespace pge {
namespace crypt {

    std::string string(std::string _string, char* key = nullptr);
    std::wstring string(std::wstring _string, char* key = nullptr);
    bool file(std::string _location, char* key = nullptr);
    bool file_ex(std::string pathi, std::string patho, char* key = nullptr);
}

namespace random {
    bool set_seed(uint32_t seed = (uint32_t)(-1));
    int get(int min, int max);
}
}
