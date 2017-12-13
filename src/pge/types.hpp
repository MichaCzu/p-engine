// Engine variables definitions.
#ifndef PGE_TYPES_HPP
#define PGE_TYPES_HPP

namespace pge {
struct Pixel {
    float x;
    float y;

    Pixel(int _x = 0, int _y = 0)
    {
        x = _x;
        y = _y;
    }
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
