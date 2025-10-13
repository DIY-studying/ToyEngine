#include "Vec2.h"
#include <cmath>

Vec2 Vec2::angleToUint(float angle)
{
	float radians= angle * (3.14/ 180.0);
	float x=std::cos(radians);
	float y = std::sin(radians);
	return Vec2(x,y);
}

Vec2::Vec2(float xin, float yin)
	:x(xin),y(yin)
{
}

Vec2::Vec2(float xin)
	:x(xin),y(xin)
{
}

Vec2::Vec2(const sf::Vector2u& v)
	:x(v.x),y(v.y)
{
}

Vec2::Vec2(const sf::Vector2i& v)
	:x(v.x), y(v.y)
{
}

bool Vec2::operator==(const Vec2& rhs) const
{
	if (rhs.x == x && rhs.y == y)
		return true;
	return false;
}

bool Vec2::operator!=(const Vec2& rhs) const
{
	return !(*this == rhs);
}

Vec2 Vec2::operator+(const Vec2& rhs) const
{
	return Vec2(x+ rhs.x,y+rhs.y);
}

Vec2 Vec2::operator-(const Vec2& rhs) const
{
	return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator*(float val) const
{
	return Vec2(val*x,val*y);
}

Vec2 Vec2::operator/(float val) const
{
	return Vec2(x/val,y/val);
}

Vec2& Vec2::operator+=(const Vec2& rhs) 
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}

Vec2& Vec2::operator-=(const Vec2& rhs) 
{
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

Vec2& Vec2::operator*=(float val) 
{
	x*= val;
	y*= val;
	return *this;
}

Vec2& Vec2::operator/=(float val) 
{
	x /= val;
	y /= val;
	return *this;
}

float Vec2::dist(const Vec2& v) const
{
	Vec2 d = *this - v;
	return sqrt(d.x*d.x+d.y*d.y);
}

float Vec2::length() const
{
	return sqrt(x * x + y * y);
}

Vec2& Vec2::normalize()
{
	float len = length();
	if (len)
	{
		x /= len;
		y /= len;
	}
	return *this;
}

