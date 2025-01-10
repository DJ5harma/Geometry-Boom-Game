#include "vec2.h"
#include "cmath"
#include <iostream>

bool vec2::operator == (const vec2& rhs) const 
{ 
	return x == rhs.x && y == rhs.y;
}

bool vec2::operator != (const vec2& rhs) const
{
	return x != rhs.x || y != rhs.y;
}

vec2 vec2::operator + (const vec2& rhs) const
{
	return vec2(x + rhs.x, y + rhs.y);
}

vec2 vec2::operator - (const vec2& rhs) const
{
	return vec2(x - rhs.x, y - rhs.y);
}

vec2 vec2::operator * (const float val) const
{
	return vec2(x * val, y * val);
}

vec2 vec2::operator / (const float val) const
{
	return vec2(x / val, y / val);
}

void vec2::operator += (const vec2& rhs)
{
	x += rhs.x; y += rhs.y;
}

void vec2::operator -= (const vec2& rhs)
{
	x -= rhs.x; y -= rhs.y;
}

void vec2::operator *= (const float val)
{
	x *= val; y *= val;
}

void vec2::operator /= (const float val)
{
	x /= val; y /= val;
}

float vec2::length() const
{
	return sqrtf(x * x + y * y);
}

float vec2::dist(const vec2& rhs) const
{
	return sqrtf(distSq(rhs));
}

float vec2::distSq(const vec2& rhs) const
{
	return (x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y);
}

void vec2::normalize()
{
	const float len = sqrtf(x * x + y * y);
	x /= len;
	y /= len;
}
void vec2::print()
{
	std::cout << "(x: " << x << ", y: " << y << ')'<<std::endl;
}
