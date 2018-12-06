#include "app/state_menu.hpp"
#include "pge/debug.hpp"
#include "pge/pge.hpp"
#include <iostream>
#include <stdint.h>

#include "menu/widget_simple.hpp"

void State_Menu::init()
{
}

void State_Menu::cleanup()
{
}

//WORKS PERFECTLY
void pop_back_utf8(std::string& utf8)
{
    if (utf8.empty())
        return;

    auto cp = utf8.data() + utf8.size();
    while (--cp >= utf8.data() && ((*cp & 0b10000000) && !(*cp & 0b01000000))) {
        if (cp >= utf8.data())
            utf8.resize(cp - utf8.data());
    }
}
//WORKS PERFECTLY

void State_Menu::handle_events(SDL_Event _event)
{
    pge::input::handle_input_event(_event);

    if (_event.type == SDL_MOUSEWHEEL) {
        font_size += _event.wheel.y;
        fTiny.resize(pge::text::tiny + font_size);
        fSmall.resize(pge::text::small + font_size);
        fNormal.resize(pge::text::normal + font_size);
        fBig.resize(pge::text::big + font_size);
        fHuge.resize(pge::text::huge + font_size);
        fExtra.resize(pge::text::extra + font_size);
    }
    AxisXL = input::state(ea_movex);
    AxisYL = input::state(ea_movey);
    AxisXR = input::state(ea_viewx);
    AxisYR = input::state(ea_viewy);

    ButtA = input::state(ec_apply) || input::state(ec_interact);
    ButtB = input::state(ec_back);
    ButtY = input::state(ec_inventory);
    ButtX = input::state(ec_item);
    TrigL2 = input::state(ea_trigl);
    TrigR2 = input::state(ea_trigr);
    TrigL1 = input::state(ec_block);
    TrigR1 = input::state(ec_attack);
    ButtSel = input::state(ec_uselessback);
    ButtSrt = input::state(ec_menu);
    DPadL = input::state(ec_skill1);
    DPadU = input::state(ec_skill2);
    DPadR = input::state(ec_skill3);
    DPadD = input::state(ec_trinket);
    ButtRS = input::state(ec_useless2);
    ButtLS = input::state(ec_useless1);
}

void State_Menu::update()
{
    pge::Pixel ctrlVect = pge::Pixel(AxisXL, AxisYL).deadzone(0.25f).trim(1.f);
    const float slrate = 0.04f, vel = 0.05f, velmax = 1.f;
    pge::Pixel spdup = (moveVect.magnitude() / velmax * slrate) < ctrlVect.magnitude() ? ctrlVect * vel : pge::Pixel(0, 0);
    moveVect = ((moveVect - (ctrlVect.magnitude() < 0.5 ? (moveVect * slrate) : pge::Pixel::Zero())) + spdup).trim(velmax);
    player = player + moveVect;

    pge::Pixel _viewdir = pge::Pixel(AxisXR, AxisYR).deadzone(0.25f);
    if (_viewdir.magnitude() > 0.5)
        viewdir = _viewdir.mod(1.f);
    else if (moveVect.magnitude() > 0.2)
        viewdir = moveVect.mod(1.f);
    else if (!pge::input::alt_mode())
        viewdir = pge::Pixel(pge::mouse::x() - player.x, pge::mouse::y() - player.y).mod(1);
}

void State_Menu::draw()
{
    pge::draw::rectF(0, 0, pge::window::w(), pge::window::h(), SDL_Color{ 50, 80, 50, 255 });

    pge::Pixel vect = pge::Pixel(AxisXL, AxisYL);
    pge::Pixel trimmed = vect.deadzone(0.25f).trim(1.f);
    pge::Pixel vall = trimmed * 128.f;
    pge::draw::circleF(pge::window::w() / 2, pge::window::h() / 2, 128, SDL_Color{ 100, 155, 255, 35 });
    pge::draw::circle(pge::window::w() / 2, pge::window::h() / 2, 128, SDL_Color{ 100, 155, 255, 155 });
    pge::draw::line(pge::window::w() / 2, pge::window::h() / 2, pge::window::w() / 2 + vall.x, pge::window::h() / 2 + vall.y);
    pge::draw::circleF(pge::window::w() / 2 + vall.x, pge::window::h() / 2 + vall.y, 2);

    pge::draw::circleF(pge::window::w() / 4, pge::window::h() / 3, pge::window::w() / 12, SDL_Color{ 200, 200, 255, 55 });
    if (ButtLS)
        pge::draw::circleF(pge::window::w() / 4 + AxisXL * pge::window::w() / 12, pge::window::h() / 3 + AxisYL * pge::window::w() / 12, 10, SDL_Color{ 200, 200, 255, 255 });
    else
        pge::draw::circleF(pge::window::w() / 4 + AxisXL * pge::window::w() / 12, pge::window::h() / 3 + AxisYL * pge::window::w() / 12, 15, SDL_Color{ 255, 255, 255, 255 });

    pge::draw::circleF(pge::window::w() * 3 / 4, pge::window::h() * 2 / 3, pge::window::w() / 12, SDL_Color{ 200, 200, 255, 55 });
    if (ButtRS)
        pge::draw::circleF(pge::window::w() * 3 / 4 + AxisXR * pge::window::w() / 12, pge::window::h() / 3 * 2 + AxisYR * pge::window::w() / 12, 10, SDL_Color{ 200, 200, 255, 255 });
    else
        pge::draw::circleF(pge::window::w() * 3 / 4 + AxisXR * pge::window::w() / 12, pge::window::h() / 3 * 2 + AxisYR * pge::window::w() / 12, 15, SDL_Color{ 255, 255, 255, 255 });

    pge::draw::rectRF(pge::window::w() / 4, 0, pge::window::w() / 12, TrigL2 * pge::window::h() / 8, 4, SDL_Color{ 255, 255, 255, 255 });
    pge::draw::rectRF(pge::window::w() * 3 / 4, 0, -pge::window::w() / 12, TrigR2 * pge::window::h() / 8, 4, SDL_Color{ 255, 255, 255, 255 });

    if (DPadU)
        pge::draw::rectRF(32, pge::window::h() - 96, 32, 32, 8, SDL_Color{ 255, 255, 255, 255 });
    if (DPadD)
        pge::draw::rectRF(32, pge::window::h() - 32, 32, 32, 8, SDL_Color{ 255, 255, 255, 255 });
    if (DPadL)
        pge::draw::rectRF(0, pge::window::h() - 64, 32, 32, 8, SDL_Color{ 255, 255, 255, 255 });
    if (DPadR)
        pge::draw::rectRF(64, pge::window::h() - 64, 32, 32, 8, SDL_Color{ 255, 255, 255, 255 });
    if (ButtSrt)
        pge::draw::rectRF(pge::window::w() / 2 + 16, pge::window::h() - 32, 32, 32, 8, SDL_Color{ 255, 255, 255, 255 });
    if (ButtSel)
        pge::draw::rectRF(pge::window::w() / 2 - 48, pge::window::h() - 32, 32, 32, 8, SDL_Color{ 255, 255, 255, 255 });
    if (ButtGud)
        pge::draw::rectRF(pge::window::w() / 2 - 16, pge::window::h() - 32, 32, 32, 8, SDL_Color{ 255, 255, 255, 255 });
    if (ButtA)
        pge::draw::rectRF(pge::window::w() - 64, pge::window::h() - 32, 32, 32, 8, SDL_Color{ 255, 255, 255, 255 });
    if (ButtB)
        pge::draw::rectRF(pge::window::w() - 32, pge::window::h() - 64, 32, 32, 8, SDL_Color{ 255, 255, 255, 255 });
    if (ButtY)
        pge::draw::rectRF(pge::window::w() - 64, pge::window::h() - 96, 32, 32, 8, SDL_Color{ 255, 255, 255, 255 });
    if (ButtX)
        pge::draw::rectRF(pge::window::w() - 96, pge::window::h() - 64, 32, 32, 8, SDL_Color{ 255, 255, 255, 255 });
    if (TrigL1)
        pge::draw::rectRF(pge::window::w() / 4, pge::window::h() / 8, pge::window::w() / 12, 32, 4, SDL_Color{ 255, 255, 255, 255 });
    if (TrigR1)
        pge::draw::rectRF(pge::window::w() * 3 / 4, pge::window::h() / 8, -pge::window::w() / 12, 32, 4, SDL_Color{ 255, 255, 255, 255 });

    if (pge::input::alt_mode()) {
        pge::draw::circleF(player.x + viewdir.x * 16, player.y + viewdir.y * 16, 10, SDL_Color{ 155, 255, 100, 255 });
        pge::draw::circleF(player.x, player.y, 18, SDL_Color{ 155, 255, 100, 255 });
        pge::draw::line(player.x, player.y, player.x + viewdir.x * 32, player.y + viewdir.y * 32, SDL_Color{ 0, 0, 0, 255 });
    } else {
        pge::draw::circleF(player.x + viewdir.x * 16, player.y + viewdir.y * 16, 10, SDL_Color{ 255, 155, 100, 255 });
        pge::draw::circleF(player.x, player.y, 18, SDL_Color{ 255, 155, 100, 255 });
        pge::draw::line(player.x, player.y, player.x + viewdir.x * 32, player.y + viewdir.y * 32, SDL_Color{ 0, 0, 0, 255 });
    }

    pge::draw::line(player.x, player.y, player.x + moveVect.x * 32, player.y + moveVect.y * 32);

    std::wstring tst = std::to_wstring(moveVect.magnitude()) + L" " + std::to_wstring(viewdir.angle() / M_PI * 180) + L"\n "
        + std::to_wstring(vect.magnitude()) + L" -> " + std::to_wstring(trimmed.magnitude());
    //std::wstring tst = L"Zażółć gęślą jaźń. " + std::to_wstring((ContVal).magnitude());
    float r = -fHuge.get_height(tst);
    pge::draw::rectF(pge::mouse::x() - fHuge.get_width(tst) / 2, pge::mouse::y(), fHuge.get_width(tst), r, SDL_Color{ 0, 0, 0, 55 });
    fHuge.drawMid(pge::mouse::x(), pge::mouse::y() + r, tst);

    fNormal.draw(0, 0, std::to_wstring(pge::get_fps()));
}
