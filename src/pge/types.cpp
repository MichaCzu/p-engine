#include "pge/types.hpp"
#include <SDL2/SDL.h>

namespace pge {
	//gets the width of a vector
	float Pixel::magnitude()
	{
		return SDL_sqrt(x * x + y * y);
	}

	//creates pixel from an angle
	Pixel Pixel::fromAngle(float r, float a)
	{
		return Pixel(SDL_cos(a) * r, SDL_sin(a) * r);
	}

	//rotates a vector through point 0,0
	Pixel Pixel::rotate(float a)
	{
		return this->fromAngle(this->magnitude(), this->angle() + a);
	}

	//removes input under a certain value
	Pixel Pixel::deadzone(float zone)
	{
		if (this->magnitude() >= zone)
			return Pixel(this->x * (abs(this->x) + zone) / (1.0 - zone), this->y * (abs(this->y) + zone) / (1.0 - zone));
		else
			return Pixel::Zero();
	}


	Pixel Pixel::mod(float m)
	{
		float am = this->magnitude();
		if (am == 0)
			return Pixel(0, 0);
		else
			return Pixel(x * m / am, y * m / am);
	}

	Pixel Pixel::trim(float m)
	{
		float am = this->magnitude();
		if (am <= m)
			return *this;
		else if (am == 0)
			return Pixel(0, 0);
		else
			return Pixel(x * m / am, y * m / am);
	}

	float Pixel::angle(Pixel p)
	{
		float a = this->magnitude() * p.magnitude();
		return SDL_acosf((*this * p - 0.001f) / a);
	}

	float Pixel::angle()
	{
		return SDL_atan2(this->y, this->x);
	}

	float Pixel::distanceTo(Pixel& that)
	{
		return Pixel(that.x - this->x, that.y - this->y).magnitude();
	}

	float Pixel::distance(Pixel& s1, Pixel& s2)
	{
		return Pixel(s2.x - s1.y, s2.y - s1.y).magnitude();
	}
}