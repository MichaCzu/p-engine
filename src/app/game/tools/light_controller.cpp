#include "light_controller.hpp"
#include "app/game/controller.hpp"

const int val = 3, val2 = 4;

void LController::reset()
{

    pge::image::query(lighttexture, ceil(viewport->w / val) + 6, ceil(viewport->h / val) + 6);
    pge::image::set_filter(lighttexturef, GPU_FILTER_LINEAR);

    pge::image::query(lighttexturef, (ceil(viewport->w / val) + 6) * 2, (ceil(viewport->h / val) + 6) * 2);
    //pge::image::set_blendfunc(lighttexturef, GPU_FUNC_DST_COLOR, GPU_FUNC_SRC_COLOR, GPU_FUNC_DST_ALPHA, GPU_FUNC_DST_ALPHA);
    //pge::image::set_blendeq(lighttexturef, GPU_EQ_ADD, GPU_EQ_ADD);
    //pge::image::set_filter(lighttexturef, GPU_FILTER_NEAREST);
}

bool LController::load_colormap(uint8_t val)
{
    pge::image::set_target(colormaptexture);
    for (int i = 50; i >= 0; i--) {
        colormap[i] = pge::draw::get_pixel(val, 50 - i);
    }
    pge::image::reset_target();
    return true;
}

SDL_Color LController::get_color(uint8_t inten)
{
    if (inten > 50)
        return colormap[50];
    else if (inten < 1)
        return colormap[0];
    else
        return colormap[inten];
}

void LController::update() //
{
    float at = 1; //abs((float(pge::get_time_passed() % 60000) - 30000) / 30000) * 5 - 3;
    if (at > 1)
        at = 1;
    if (at < 0)
        at = 0;

    load_colormap(at * 40);
    skylight = abs(at * 41 + 9);

    pge::image::set_target(lighttexture);
    pge::window::clear();

    float px, py;
    control->plr()->get_pos(&px, &py);
    //SDL_GetMouseState(&mx, &my);

    int shx = viewport->x / val;
    int shy = viewport->y / val;

    offset_x = floor((viewport->x) / val) * val;
    offset_y = floor((viewport->y) / val) * val;

    int mx, my;
    SDL_GetMouseState(&mx, &my);
    //mx = mx / pge::draw::get_scale() / 4 + 1;
    //my = my / pge::draw::get_scale() / 4 + 1;

    vLights[0].x = (px) / val;
    vLights[0].y = (py - 8) / val;

    vLights[1].x = mx / pge::draw::get_scale() / val + shx;
    vLights[1].y = my / pge::draw::get_scale() / val + shy;
    //int x1 = (mx / pge::draw::get_scale() - int(viewport->x) % 12) / 12;
    //int y1 = (my / pge::draw::get_scale() - int(viewport->y) % 12) / 12;

    for (int i = -val2; i < pge::image::get_height(lighttexture); i++)
        for (int j = -val2; j < pge::image::get_width(lighttexture); j++) {
            uint8_t light = 10; // = intensity_test(j + shx, i + shy);
            for (int k = vLights.size() - 1; k >= 0; k--) {
                if (k > 1)
                    vLights[k].intensity = skylight;
                if (light < vLights[k].intensity)
                    if (abs(j + shx - vLights[k].x) < vLights[k].intensity && abs(i + shy - vLights[k].y) < vLights[k].intensity) {
                        uint8_t test = intensity_test(j + shx, i + shy, vLights[k].x, vLights[k].y, vLights[k].intensity);
                        if (light < test)
                            light = test;
                    }
                if (light > 49)
                    break;
            }
            //light = best;
            light += 5;
            if (light > 51)
                light = 51;

            SDL_Color color = SDL_Color{ 100, 100, 100, light * 5 };
            pge::draw::pixel(j + val2, i + val2, color);
        }
    //pge::draw::pixel_color(1, 1, 0xFFFFFFFF);
    //pge::draw::pixel_color(0, 0, 0xFFFFFFFF);
    pge::image::set_target(lighttexturef);
    pge::window::clear();
    pge::image::draw(lighttexture, 0, 0, NULL, NULL, 0, 2, 2);
    pge::image::reset_target();
}

int LController::intensity_test(int x1, int y1)
{
    const int rang = 1;

    for (int x = -rang; x <= rang; x++)
        for (int y = -rang; y <= rang; y++)
            if (control->env()->bg_check((x1 + x * 2) / val2, (y1 + y * 2) / val2) != bt_normal && control->env()->bc_check((x1 + x * 2) / val2, (y1 + y * 2) / val2) != bt_normal)
                return skylight;

    return 0;
}

int LController::intensity_test(int x0, int y0, int x1, int y1, float inten)
{
    int flag = 0;
    if (x0 == x1 && y0 == y1)
        return inten > 50 ? 50 : inten;
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;
    bool out = control->env()->bc_check(x0 / val2, y0 / val2) != bt_normal;

    uint8_t isa = 0;
    while (true) {
        bool xx = false;
        bool yy = false;

        int xd = x0 / val2;
        int yd = y0 / val2;

        if (out && control->env()->bc_check(xd, yd) == bt_normal) {
            return 0;
        } else if (!out && control->env()->bc_check(xd, yd) != bt_normal) {
            out = true;
            inten += 10;
        }

        if ((x0 == x1 && y0 == y1) || inten <= 0) {
            return flag = inten < 0 ? 0 : (inten > 50 ? 50 : inten);
            //break;
        }
        int e2 = 2 * err;

        if (e2 < dx) {
            err = err + dx;
            y0 += sy;
            yy = true;
        }
        if (e2 > -dy) {
            err = err - dy;
            x0 += sx;
            xx = true;
        }

        if (!out) {
            if (xx || yy)
                inten -= 20;
        } else {
            if (xx && yy)
                inten -= 1.4;
            else
                inten -= 1;
        }
    }
}

void LController::draw()
{
    float var = val * pge::draw::get_scale() / 2;
    float s = pge::draw::get_scale();

    float offx = (offset_x - viewport->x) * s;
    float offy = (offset_y - viewport->y) * s;

    if (switch1)
        pge::image::draw(lighttexturef, offx - (val2 * var) * 2, offy - (val2 * var) * 2, NULL, NULL, 0, var, var);
}
