// Engine variables definitions.
#pragma once
#include <algorithm>
#include <vector>

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
	//Pixel(Pixel& p1)
	//{
		//x = p1.x;
		//y = p1.y;
	//}

    static Pixel fromAngle(float r, float a);
    float distanceTo(Pixel& that);
    static float distance(Pixel& s1, Pixel& s2);

    float magnitude();
    Pixel deadzone(float zone);
    Pixel trim(float length);
    Pixel mod(float m);
    Pixel rotate(float a);
    float angle(Pixel p);
    float angle();

    Pixel operator+=(Pixel other) { return Pixel(this->x + other.x, this->y + other.y); }
    Pixel operator+(Pixel other) { return Pixel(this->x + other.x, this->y + other.y); }
    Pixel operator-=(Pixel other) { return Pixel(this->x - other.x, this->y - other.y); }
    Pixel operator-(Pixel other) { return Pixel(this->x - other.x, this->y - other.y); }
    float operator*(Pixel other) { return this->x * other.x + this->y * other.y; }
    Pixel operator*(double other) const { return Pixel(this->x * other, this->y * other); }
    Pixel operator*=(double other) const { return Pixel(this->x * other, this->y * other); }
	
	bool operator==(Pixel other) const { return (this->x == other.x && this->y == other.y); }
	bool operator!=(Pixel other) const { return !(*this == other); }

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

template <typename T>
class Matrix2D
{
	std::vector<T> inner_;
//	std::vector<int> inner_;
	unsigned int width, height;

public:
	Matrix2D(unsigned int _width, unsigned int _height)
		: width(_width), height(_height)
	{
		inner_.resize(_width * _height);
	}

	T& operator()(unsigned int x, unsigned int y)
	{
		//if (x >= width || y >= height)
			//throw std::out_of_range("matrix indices out of range"); // ouch
		return inner_[width * y + x];
	}

	bool check(unsigned int x, unsigned int y)
	{
		return (x >= width || y >= height);
	}
	unsigned int w() { return width; }
	unsigned int h() { return height; }
};

}
