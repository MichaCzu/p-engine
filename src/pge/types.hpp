// Engine variables definitions.
#ifndef PGE_TYPES_HPP
#define PGE_TYPES_HPP
#include <algorithm>

namespace pge {
struct Pixel {
    float x;
    float y;

    Pixel()
    {
        x = 0;
        y = 0;
    }
    Pixel(float _x, float _y)
    {
        x = _x;
        y = _y;
    }
    Pixel(float _x, float _y, float _x2, float _y2)
    {
        x = _x2 - _x;
        y = _y2 - _y;
    }
    Pixel(Pixel p1, Pixel p2)
    {
        x = p2.x - p1.x;
        y = p2.y - p1.y;
    }

    float distanceTo(Pixel& that);
    static float distance(Pixel& s1, Pixel& s2);

    float magnitude();
    Pixel deadzone(float zone);
    Pixel trim(float length);
    Pixel mod(float m);
    float angle();

    Pixel operator+=(Pixel other) { return Pixel(this->x + other.x, this->y + other.y); }
    Pixel operator+(Pixel other) { return Pixel(this->x + other.x, this->y + other.y); }
    Pixel operator-=(Pixel other) { return Pixel(this->x - other.x, this->y - other.y); }
    Pixel operator-(Pixel other) { return Pixel(this->x - other.x, this->y - other.y); }
    float operator*(Pixel other) { return this->x * other.x + this->y * other.y; }
    Pixel operator*(double other) const { return Pixel(this->x * other, this->y * other); }
    Pixel operator*=(double other) const { return Pixel(this->x * other, this->y * other); }

    static Pixel Zero() { return Pixel(0, 0); }
};

struct Rect {
    float x;
    float y;
    float w;
    float h;

    Rect()
    {
        x = 0;
        y = 0;
        w = 0;
        h = 0;
    }
    Rect(float _x, float _y, float _w, float _h)
    {
        x = _x;
        y = _y;
        w = _w;
        h = _h;
    }
};

struct Clip {
    Rect r;
    Pixel p;

    Clip()
    {
    }
    Clip(float px, float py, float _x, float _y, float _w, float _h)
    {
        p.x = px;
        p.y = py;
        r.x = _x;
        r.y = _y;
        r.w = _w;
        r.h = _h;
    }
};
}

#endif // TYPES_HPP
