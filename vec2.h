#pragma once
class vec2
{
public:

	float x;
	float y;

	vec2() :x( 0 ), y( 0 ) {};
	vec2(float xin, float yin) :x{ xin }, y{ yin } {}

	bool operator == (const vec2& rhs) const;
	bool operator != (const vec2& rhs) const;

	vec2 operator + (const vec2& rhs) const;
	vec2 operator - (const vec2& rhs) const;
	vec2 operator * (const float val) const;
	vec2 operator / (const float val) const;

	void operator += (const vec2& rhs);
	void operator -= (const vec2& rhs);
	void operator *= (const float val);
	void operator /= (const float val);

	float length() const;
	float dist(const vec2 & rhs) const;
	float distSq(const vec2& rhs) const; // skips sqrt to improve speed for distance comparison checks
	void normalize(); // makes it a unit vector
	void print();
};

