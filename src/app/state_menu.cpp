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
    }
    if (cp >= utf8.data())
        utf8.resize(cp - utf8.data());
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

    ButtA = input::state(ec_apply);
    ButtB = input::state(ec_back);
    ButtX = input::state(ec_trinket);
    ButtY = input::state(ec_item);
    TrigL2 = input::state(ea_trigl);
    TrigR2 = input::state(ea_trigr);
    //TrigL1 = input::state(ec_trinket);
    //TrigR1 = input::state(ec_item);
    ButtSel = input::state(ec_inventory);
    ButtSrt = input::state(ec_menu);
    DPadL = input::state(ec_skill1);
    DPadU = input::state(ec_skill2);
    DPadR = input::state(ec_skill3);
}

void State_Menu::update()
{
}

void State_Menu::draw()
{
    pge::draw::rectF(0, 0, pge::window::w() / 4 * 3, pge::window::h() / 4 * 3, SDL_Color{ 50, 50, 50, 255 });
    pge::draw::rectF(0, 0, pge::window::w() / 2, pge::window::h() / 2, SDL_Color{ 100, 100, 100, 255 });
    pge::draw::rectF(0, 0, pge::window::w() / 4, pge::window::h() / 4, SDL_Color{ 150, 150, 150, 255 });
    pge::draw::rectF(0, 0, pge::window::w() / 8, pge::window::h() / 8, SDL_Color{ 200, 200, 200, 255 });
    pge::draw::rectF(0, 0, pge::window::w() / 16, pge::window::h() / 16, SDL_Color{ 230, 230, 230, 255 });
    pge::draw::rectF(pge::window::w(), pge::window::h(), -pge::window::w() / 4, -pge::window::h() / 4, SDL_Color{ 50, 50, 50, 255 });

    float vallX, vallY, vallA;
    vallA = sqrt(AxisYL * AxisYL + AxisXL * AxisXL);
    vallX = std::fmin(1.f, vallA) * 64.f * AxisXL / vallA;
    vallY = std::fmin(1.f, vallA) * 64.f * AxisYL / vallA;
    pge::draw::line(pge::window::w() / 2, pge::window::h() / 2, pge::window::w() / 2 + vallX, pge::window::h() / 2 + vallY);

    if (ButtLS)
        pge::draw::circleF(pge::window::w() / 4 + AxisXL * pge::window::w() / 12, pge::window::h() / 3 + AxisYL * pge::window::w() / 12, 20, SDL_Color{ 200, 200, 255, 255 });
    else
        pge::draw::circleF(pge::window::w() / 4 + AxisXL * pge::window::w() / 12, pge::window::h() / 3 + AxisYL * pge::window::w() / 12, 20, SDL_Color{ 255, 255, 255, 255 });

    if (ButtRS)
        pge::draw::circleF(pge::window::w() * 3 / 4 + AxisXR * pge::window::w() / 12, pge::window::h() / 3 * 2 + AxisYR * pge::window::w() / 12, 20, SDL_Color{ 200, 200, 255, 255 });
    else
        pge::draw::circleF(pge::window::w() * 3 / 4 + AxisXR * pge::window::w() / 12, pge::window::h() / 3 * 2 + AxisYR * pge::window::w() / 12, 20, SDL_Color{ 255, 255, 255, 255 });

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

    float ContVal = sqrt(AxisXL * AxisXL + AxisYL * AxisYL);
    std::wstring tst = L"Zażółć gęślą jaźń. " + std::to_wstring(ContVal);
    float r = 0;
    float rr = fExtra.get_height(tst) + fHuge.get_height(tst) + fBig.get_height(tst) + fTiny.get_height(tst) + fSmall.get_height(tst) + fNormal.get_height(tst);
    pge::draw::rectF(pge::mouse::x() - fExtra.get_width(tst) / 2, pge::mouse::y(), fExtra.get_width(tst), -rr, SDL_Color{ 0, 0, 0, 55 });
    r += fTiny.get_height(tst);
    fTiny.drawMid(pge::mouse::x(), pge::mouse::y() - r, tst);
    r += fSmall.get_height(tst);
    fSmall.drawMid(pge::mouse::x(), pge::mouse::y() - r, tst);
    r += fNormal.get_height(tst);
    fNormal.drawMid(pge::mouse::x(), pge::mouse::y() - r, tst);
    r += fBig.get_height(tst);
    fBig.drawMid(pge::mouse::x(), pge::mouse::y() - r, tst);
    r += fHuge.get_height(tst);
    fHuge.drawMid(pge::mouse::x(), pge::mouse::y() - r, tst);
    r += fExtra.get_height(tst);
    fExtra.drawMid(pge::mouse::x(), pge::mouse::y() - r, tst);

    fNormal.draw(0, 0, std::to_wstring(pge::get_fps()));
}
