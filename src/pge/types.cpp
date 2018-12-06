#include "types.hpp"
#include <SDL2/SDL.h>

float pge::Pixel::magnitude()
{
    return SDL_sqrt(x * x + y * y);
}

pge::Pixel pge::Pixel::deadzone(float zone)
{
    if (this->magnitude() >= zone)
        return pge::Pixel(this->x * (abs(this->x) + zone) / (1.0 - zone), this->y * (abs(this->y) + zone) / (1.0 - zone));
    else
        return pge::Pixel::Zero();
}

pge::Pixel pge::Pixel::mod(float m)
{
    float am = this->magnitude();
    if (am == 0)
        return pge::Pixel(0, 0);
    else
        return pge::Pixel(x * m / am, y * m / am);
}

pge::Pixel pge::Pixel::trim(float m)
{
    float am = this->magnitude();
    if (am <= m)
        return *this;
    else if (am == 0)
        return pge::Pixel(0, 0);
    else
        return pge::Pixel(x * m / am, y * m / am);
}

float pge::Pixel::angle()
{
    return SDL_atan2(this->y, this->x);
}

float pge::Pixel::distanceTo(Pixel& that)
{
    return Pixel(that.x - this->x, that.y - this->y).magnitude();
}

float pge::Pixel::distance(Pixel& s1, Pixel& s2)
{
    return Pixel(s2.x - s1.y, s2.y - s1.y).magnitude();
}
