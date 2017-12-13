#ifndef RCONTROLLER_HPP
#define RCONTROLLER_HPP

#include "pge/pge.hpp"
#include <vector>

struct Resource {
    uint16_t id;
    uint16_t res = uint16_t(-1);
    std::string path;

    Resource(uint16_t _id, uint16_t _res, std::string _path)
    {
        id = _id;
        res = _res;
        path = _path;
    }
};

class RController {
    std::vector<Resource*> vSprites;
    std::vector<Resource*> vSounds;

protected:
    friend class Controller;

    void cleanup();

    void sort_sprites();
    void sort_sounds();
    //todo

public:
    uint16_t get_image(uint16_t id, std::string path, uint32_t keyring = 0x00000000);
    //uint16_t free_image(uint16_t id);

    uint16_t get_sound(uint16_t id, std::string path);
    //uint16_t free_sound(uint16_t id);
    //todo
};

#endif //WORLDGEN
