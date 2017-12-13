#include "resource_controller.hpp"
#include "pge/image.hpp"
#include "pge/sound.hpp"
#include <stdint.h>

void RController::cleanup()
{
    for (int i = 0; i > vSprites.size(); i++) {
        pge::image::free(vSprites[i]->res);
    }
    for (int i = 0; i > vSounds.size(); i++) {
        pge::image::free(vSounds[i]->res);
    }
}

void RController::sort_sprites()
{
}

void RController::sort_sounds()
{
}
//todo

uint16_t RController::get_image(uint16_t _id, std::string path, uint32_t keyring)
{
    int start = 0;
    int end = vSprites.size() - 1;
    int mid = 0;

    if (vSprites.size() > 0) {
        while (start <= end) {
            mid = (start + end) / 2;
            if (_id == vSprites[mid]->id)
                return vSprites[mid]->res;
            else if (_id < vSprites[mid]->id)
                end = mid - 1;
            else
                start = mid + 1;
        }
    }

    uint16_t flag = pge::image::load(path);
    if (flag != uint16_t(-1)) {
        vSprites.insert(vSprites.begin() + mid, new Resource(_id, flag, path));
    }
    return flag;
}

uint16_t RController::get_sound(uint16_t id, std::string path)
{
    int start = 0;
    int end = vSprites.size() - 1;
    int mid = 0;

    if (vSprites.size() > 0) {
        while (start <= end) {
            mid = (start + end) / 2;
            if (id == vSprites[mid]->id)
                return vSprites[mid]->res;
            else if (id < vSprites[mid]->id)
                end = mid - 1;
            else
                start = mid + 1;
        }
    }

    uint16_t flag = pge::chunk::load(path);
    if (flag != uint16_t(-1)) {
        vSprites.insert(vSprites.begin() + mid, new Resource(id, flag, path));
    }
    return flag;
}
