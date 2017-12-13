#include "app/game/controller.hpp"
#include "app/game/entities/entity.hpp"
#include "app/game/entities/player/player.hpp"
#include "app/game/enviroment_controller.hpp"
#include "app/game/particles/particle.hpp"
#include "app/game/particles/particle_block.hpp"
#include "app/game/particles/particle_rain.hpp"
#include "app/game/player/item.hpp"
#include "app/game/player_controller.hpp"
#include "pge/pge.hpp"
#include <vector>
//object management

static int controlBlock = 0;
static int sel_block = 1;

Controller::Controller()
    : Pcontrol(this, 1200, 124 * 12)
    , Lcontrol(this, &viewport)
{
    aBlocks_Init();
    aItems_Init();

    float xx, yy, scale = pge::draw::get_scale();
    int w, h;
    Pcontrol.get_pos(&xx, &yy);
    pge::window::get_size(&w, &h);

    viewport.w = w / scale;
    viewport.h = h / scale;
    viewport.x = xx - viewport.w / 2;
    viewport.y = yy - viewport.h / 3 * 2;
    //Lcontrol.reset();

    fronttexture = pge::image::create(w, h);
}

Controller::~Controller()
{
    for (int i = 0; i < vEntities.size(); i++) {
        delete vEntities[i];
    }
    vEntities.erase(vEntities.begin(), vEntities.end());
}

void Controller::handle_event(SDL_Event _event)
{

    int xx, yy, shxx, shyy, mx, my;

    //if (_event.type == SDL_MOUSEWHEEL) {
    //if (_event.wheel.y > 0)
    //Lcontrol.bring_light(true);
    //    else if (_event.wheel.y < 0)
    //Lcontrol.bring_light(false);
    //        return;
    //}
    if (_event.type == SDL_MOUSEBUTTONDOWN) {
        switch (_event.button.button) {
        case SDL_BUTTON_LEFT:
            controlBlock = 1;
            break;
        case SDL_BUTTON_RIGHT:
            controlBlock = 2;
            break;
        case SDL_BUTTON_MIDDLE:
            controlBlock = -1;
            break;
        default:
            break;
        }
    } else if (_event.type == SDL_MOUSEBUTTONUP) {
        controlBlock = 0;
    } else if (_event.type == SDL_KEYDOWN) {
        switch (_event.key.keysym.scancode) {
        case SDL_SCANCODE_1:
            sel_block = 1;
            break;
        case SDL_SCANCODE_2:
            sel_block = 2;
            break;
        case SDL_SCANCODE_3:
            sel_block = 3;
            break;
        case SDL_SCANCODE_4:
            sel_block = 4;
            break;
        case SDL_SCANCODE_5:
            sel_block = 5;
            break;
        case SDL_SCANCODE_6:
            sel_block = 6;
            break;
        case SDL_SCANCODE_7:
            sel_block = 7;
            break;
        case SDL_SCANCODE_8:
            sel_block = 8;
            break;
        case SDL_SCANCODE_9:
            sel_block = 9;
            break;
        case SDL_SCANCODE_0:
            sel_block = 10;
            break;
        case SDL_SCANCODE_MINUS:
            sel_block = 11;
            break;
        case SDL_SCANCODE_EQUALS:
            sel_block = 12;
            break;
        case SDL_SCANCODE_BACKSPACE:
            sel_block = 13;
            break;
        case SDL_SCANCODE_W:
            sel_block = 14;
            break;
        //case SDL_SCANCODE_G:
        //Lcontrol.place_light(Lcontrol.get_lightx(1), Lcontrol.get_lighty(1), Lcontrol.get_light(1));
        //    break;
        case SDL_SCANCODE_J:
            Pcontrol.inventory.pickup_item(&aItems[ii_sword], 1);
            break;
        case SDL_SCANCODE_K:
            Pcontrol.inventory.pickup_item(&aItems[ii_apple], 1);
            break;
        case SDL_SCANCODE_L:
            Pcontrol.inventory.pickup_item(&aItems[ii_steak], 1);
            break;
        case SDL_SCANCODE_U:
            Pcontrol.inventory.pickup_item(&aItems[ii_ringblue], 1);
            break;
        case SDL_SCANCODE_I:
            Pcontrol.inventory.pickup_item(&aItems[ii_ringred], 1);
            break;
        case SDL_SCANCODE_O:
            Pcontrol.inventory.pickup_item(&aItems[ii_amulet], 1);
            break;
        case SDL_SCANCODE_P:
            Pcontrol.inventory.pickup_item(&aItems[ii_crystal], 1);
            break;
        case SDL_SCANCODE_B:
            Pcontrol.inventory.pickup_item(&aItems[ii_hat], 1);
            break;
        case SDL_SCANCODE_N:
            Pcontrol.inventory.pickup_item(&aItems[ii_shirt], 1);
            break;
        case SDL_SCANCODE_M:
            Pcontrol.inventory.pickup_item(&aItems[ii_pants], 1);
            break;
        //case SDL_SCANCODE_F5:
        //Lcontrol.switch1_();
        //    break;
        //case SDL_SCANCODE_F6:
        //Lcontrol.switch2_();
        //    break;
        default:
            break;
        }
    }
    Pcontrol.handle_event(_event);
}

void Controller::handle_create()
{
    for (int i = 0; i < vCreateCalls.size(); i++) {
        vEntities.push_back(vCreateCalls[i]);
    }
    vCreateCalls.clear();
}
void Controller::handle_destroy()
{
    for (int i = 0; i < vEntities.size(); i++)
        if (vEntities[i]->flagDestroy) {
            delete vEntities[i];
            vEntities.erase(vEntities.begin() + i);
            i--;
        }
    for (int i = 0; i < vParticles.size(); i++)
        if (vParticles[i]->flagDestroy) {
            delete vParticles[i];
            vParticles.erase(vParticles.begin() + i);
            i--;
        }
}

void Controller::handle_prelogic()
{
    Pcontrol.inventory.handle_events();
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    int vpx = viewport.x * pge::draw::get_scale(), vpy = viewport.y * pge::draw::get_scale();
    int scaled = 12 * pge::draw::get_scale();

    int xx = floor((vpx + mx) / scaled), yy = floor((vpy + my) / scaled);

    float sm_change = 0.002 * pge::get_time_difference();
    if (stopmotion_factor > stopmotion_target - sm_change && stopmotion_factor < stopmotion_target + sm_change)
        stopmotion_factor = stopmotion_target;
    else if (stopmotion_factor < stopmotion_target)
        stopmotion_factor += sm_change;
    else if (stopmotion_factor > stopmotion_target)
        stopmotion_factor -= sm_change;

    if (controlBlock == 1) {
        Econtrol.bc_place_block(sel_block, xx, yy, 0);
    } else if (controlBlock == 2) {
        Econtrol.bc_place_block(sel_block, xx, yy, 1);
    } else if (controlBlock == -1) {
        if (Econtrol.bc_check(xx, yy) == bt_normal)
            for (int i = pge::random::get(16, 32); i > 0; i--)
                create_particles(new Particle_Block(this, xx * 12 + pge::random::get(2, 10), yy * 12 + pge::random::get(2, 10), 0, 0, *Econtrol.bc_type(xx, yy)));
        Econtrol.bc_remove_block(xx, yy);
    }

    for (int i = 0; i < vEntities.size(); i++)
        vEntities[i]->prelogic(pge::get_time_difference() * stopmotion_factor);
}

void Controller::handle_collision()
{
    for (int i = 0; i < vEntities.size() - 1; i++) {
        if (vEntities[i]->hitReady)
            for (int j = i + 1; j < vEntities.size(); j++) {
                if (SDL_HasIntersection(&vEntities[i]->dmgBox, &vEntities[j]->hitBox)) {
                    vEntities[i]->collision(vEntities[j]);
                }
            }
    }
}

void Controller::handle_fixedstep()
{

    uint16_t steptime;
    uint16_t steps;

    steptime = pge::get_time_difference() * stopmotion_factor + fixedstep_rest;
    steps = steptime / 10;
    fixedstep_rest = steptime % 10;

    for (; steps > 0; steps--) {
        for (int j = 0; j < vEntities.size(); j++)
            vEntities[j]->fixedstep();
        for (int j = 0; j < vParticles.size(); j++)
            vParticles[j]->fixedlogic();
    }
}

void Controller::handle_logic()
{
    for (int i = 0; i < vEntities.size(); i++)
        vEntities[i]->logic(pge::get_time_difference() * stopmotion_factor);
}

void Controller::adjust_draw()
{
    float scale = pge::draw::get_scale();
    int w, h;
    pge::window::get_size(&w, &h);

    viewport.w = w / scale;
    viewport.h = h / scale;
    pge::image::query(fronttexture, w, h);
    pge::image::set_blendmode(fronttexture, GPU_BLEND_NORMAL);
    //Lcontrol.reset();
}

void Controller::handle_camera()
{
    float xx, yy, ty, scale = pge::draw::get_scale();

    Pcontrol.get_pos(&xx, &yy);
    ty = yy - viewport.h / 3 * 2;

    viewport.x = xx - viewport.w / 2;
    float tyy = (ty - viewport.y);
    viewport.y += tyy / abs(640 - (tyy * 2)) * pge::get_time_difference();

    if (viewport.x < 0) {
        viewport.x = 0;
    }
    if (viewport.x > Econtrol.get_mapWidth() * 12 - viewport.w) {
        viewport.x = Econtrol.get_mapWidth() * 12 - viewport.w;
    }
    if (viewport.y < 0) {
        viewport.y = 0;
    }
    if (viewport.y > Econtrol.get_mapHeight() * 12 - viewport.h) {
        viewport.y = Econtrol.get_mapHeight() * 12 - viewport.h;
    }
}
void Controller::handle_draw_background()
{
    Econtrol.bg_render(1536, viewport, pge::get_time_difference() * stopmotion_factor);
}

void Controller::handle_draw()
{
    std::vector<int> blocksToDraw;
    Econtrol.bc_renderbg(viewport, &blocksToDraw);

    std::vector<int>* flag = handle_draw_particles();

    for (int i = 0; i < vEntities.size(); i++)
        vEntities[i]->draw(viewport);
    Econtrol.bc_render(viewport, &blocksToDraw);

    handle_draw_particles_front(flag);

    /*
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    sctBlock a(&aBlocks[b_grass]);
    a.tile(0b11111111);
    int scaled = 12 * pge::draw::get_scale();
    int vpx = viewport.x * pge::draw::get_scale(), vpy = viewport.y * pge::draw::get_scale();

    pge::draw::rect_color(floor((vpx + mx) / scaled) * scaled - vpx - 1,
        floor((vpy + my) / scaled) * scaled - vpy - 1,
        scaled, scaled, 0x22CC2277);*/
}

std::vector<int>* Controller::handle_draw_particles()
{
    std::vector<int>* output = new std::vector<int>;
    for (int i = 0; i < vParticles.size(); i++) {
        if (!vParticles[i]->isfront)
            vParticles[i]->draw(viewport);
        else
            output->push_back(i);
    }
    return output;
}

void Controller::handle_draw_particles_front(std::vector<int>* part)
{
    for (int i = 0; i < part->size(); i++)
        vParticles[(*part)[i]]->draw(viewport);
    part->clear();
    delete part;
}

void Controller::handle_draw_light()
{
    //static uint32_t nexttime = 0;
    //if (nexttime < pge::get_time_passed()) {
    //    nexttime = pge::get_time_passed() + 1000 / 20;
    //Lcontrol.update();
    //}
    //pge::image::set_target(fronttexture);
    //Lcontrol.draw();
}

void Controller::handle_draw_gui()
{
    Pcontrol.draw_gui(viewport);
}

//object managementfloor
uint8_t Controller::destroy(Entity* _entity)
{
    for (int i = 0; i < vEntities.size(); i++) {
        if (vEntities[i] == _entity) {
            vEntities[i]->flagDestroy = true;
            return 0;
        }
    }

    return 1;
}
Entity* Controller::create(Entity* _entity)
{
    vEntities.push_back(_entity);
    return _entity;
}

bool Controller::block_move(float* x, float* y)
{
    bool flag = false;
    while (uint16_t i = Econtrol.bc_check(*x / 12, *y / 12) < bt_platfm) {
        //if (i > 2000 || *y + 4 < floor((*y - 1) * 12) / 12)
        *y = floor((*y - 1) * 12) / 12;
        flag = true;
    }
    if (flag)
        *y += 1;
    return flag;
}

void Controller::create_particles(Particle* _particle)
{
    vParticles.push_back(_particle);
}

bool Entity::_isFree(int type, float xoff, float yoff)
{
    return (control->env()->bc_check((x + xoff) / 12, (y + yoff) / 12) > type);
}

bool Entity::_isType(int type, float xoff, float yoff)
{
    return (control->env()->bc_check((x + xoff) / 12, (y + yoff) / 12) == type);
}

float Entity::_getModSpeed(float xoff, float yoff)
{
    return control->env()->bc_type((x + xoff) / 12, (y + yoff) / 12, false)->speed_mod;
}

float Entity::_getModSpdSlow(float xoff, float yoff)
{
    return control->env()->bc_type((x + xoff) / 12, (y + yoff) / 12, false)->spdslow_mod;
}
float Entity::_getModSpdUp(float xoff, float yoff)
{
    return control->env()->bc_type((x + xoff) / 12, (y + yoff) / 12, false)->spdup_mod;
}
float Entity::_getModJump(float xoff, float yoff)
{
    return control->env()->bc_type((x + xoff) / 12, (y + yoff) / 12, false)->jump_mod;
}
float Entity::_getModJumpSpd(float xoff, float yoff)
{
    return control->env()->bc_type((x + xoff) / 12, (y + yoff) / 12, false)->jumpspd_mod;
}

float Entity::_getModBounce(float xoff, float yoff)
{
    return control->env()->bc_type((x + xoff) / 12, (y + yoff) / 12, false)->bouncy_mod;
}

float Entity::_getModFalldmg(float xoff, float yoff)
{
    return control->env()->bc_type((x + xoff) / 12, (y + yoff) / 12, false)->falldmg_mod;
}
