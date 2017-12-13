#include "player_controller.hpp"
#include "app/game/block.hpp"
#include "app/game/controller.hpp"
#include "app/game/entities/player/player.hpp"
#include "app/game/enviroment_controller.hpp"
#include "block.hpp"
#include "pge/pge.hpp"
#include "tools/worldgen.hpp"
#include <iostream>
#include <vector>

PController::PController(Controller* _control, float _x, float _y)
    : inventory(_control)
{
    control = _control;
    gui = control->res()->get_image(401, "res/spr/hud/gui/gui.png", 0xFF00FFFF);
    player = new Player(control, _x, _y, 8 * 7, this);
    control->create(player);
}

PController::~PController()
{
}

void PController::signal_death() { player = nullptr; }
void PController::get_pos(float* _x, float* _y)
{
    player->get_pos(_x, _y);
}

void PController::handle_event(SDL_Event& e)
{
    bool _flag;

    if (e.type == SDL_KEYDOWN)
        _flag = true;
    else if (e.type == SDL_KEYUP)
        _flag = false;
    else
        return;

    SDL_Scancode key = e.key.keysym.scancode;

    if (key == SDL_SCANCODE_Q)
        player->m_hpup = _flag;
    if (key == SDL_SCANCODE_W)
        player->m_hpdrain = _flag;

    if (e.key.repeat > 0)
        return;

    if (co_check(key, ec_up)) {
        player->m_up = _flag;
        inventory.s_up = _flag;
    }
    if (co_check(key, ec_down)) {
        player->m_down = _flag;
        inventory.s_down = _flag;
    }
    if (co_check(key, ec_left)) {
        player->m_left = _flag;
        inventory.s_left = _flag;
    }
    if (co_check(key, ec_right)) {
        player->m_right = _flag;
        inventory.s_right = _flag;
    }
    if (co_check(key, ec_sprint)) {
        player->m_sprint = _flag;
        inventory.s_super = _flag;
    }
    if (co_check(key, ec_shiftfor)) {
        inventory.s_shiftfor = _flag;
    }
    if (co_check(key, ec_shiftbac)) {
        inventory.s_shiftbac = _flag;
    }
    if (co_check(key, ec_apply)) {
        inventory.s_selector = _flag;
    }
    if (co_check(key, ec_back)) {
        inventory.s_back = _flag;
    }

    if (co_check(key, ec_jump))
        player->m_jump = _flag;
    if (co_check(key, ec_inventory) && _flag) {
        inventory.show(inventory.is_shown() ? false : true);
    }
}

void PController::draw_gui(pge::viewport::View& vp)
{
    int scale = pge::draw::get_scale() + 1;
    int w, h;
    pge::window::get_size(&w, &h);
    inventory.draw_gui();

    //float tyy = (player->life - smooth_life);
    //smooth_life += (tyy / abs(169 - tyy * 3)) * pge::get_time_difference();

    for (int i = 0; i < 8; i++) {
        pge::Rect rect;
        if (i == int(player->life) / 7)
            rect = pge::Rect(49 - (int(player->life) % 7 * 7), 18, 7, 7);
        else if (i < int(player->life) / 7)
            rect = pge::Rect(0, 18, 7, 7);
        else
            rect = pge::Rect(49, 18, 7, 7);

        pge::image::draw(gui, w / 2 - ((57 - 7 * i) * scale), h - 16 - 27 * scale, &rect, NULL, 0, scale, scale);
    }

    for (int i = 0; i < 8; i++) {
        pge::Rect rect;

        if (i == int(player->energy) / 7)
            rect = pge::Rect(49 - (int(player->energy) % 7 * 7), 32 - player->tired * 7, 7, 7);
        else if (i < int(player->energy) / 7)
            rect = pge::Rect(0, 32 - player->tired * 7, 7, 7);
        else
            rect = pge::Rect(49, 32 - player->tired * 7, 7, 7);

        pge::image::draw(gui, w / 2 + ((49 - 7 * i) * scale), h - 16 - 27 * scale, &rect, NULL, 0, scale, scale);
    }
}

bool PController::check_item_requirements(Item* _item)
{
    if (_item == nullptr)
        return false;

    for (auto& _property : _item->properties) {
        switch (_property.first) {
        default:
            break;
        case ip_health:
            if (_property.second > 0 && player->life >= player->maxhealth)
                return false;
            break;
        case ip_energy:
            if (_property.second > 0 && player->energy >= player->maxhealth)
                return false;
            break;
        case ip_energy_req:
            if (player->tired)
                return false;
            break;
        case ip_health_req:
            if (player->life < _property.second)
                return false;
            break;
        }
    }
    return true;
}

void PController::use_item(Item* _item)
{
    if (_item == nullptr)
        return;

    for (auto& _property : _item->properties) {
        switch (_property.first) {
        default:
            break;
        case ip_health:
            player->life += _property.second;
            break;
        case ip_energy:
            player->energy += _property.second;
            break;
        }
    }
}
