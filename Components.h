#pragma once
#include "vec2.h"
#include <SFML/Graphics.hpp>

class CTransform
{
public:
	vec2 pos;
	vec2 velocity;
	float angle;

	CTransform(const vec2& p, const vec2& v, float a) 
		: pos(p), velocity(v), angle(a){};
};

class CShape
{
public:
	sf::CircleShape circle;
	
	CShape(float radius, int vertices, const sf::Color & fill, const sf::Color & outline, float thickness)
		//: circle(radius, vertices) 
	{
		circle.setRadius(radius);
		circle.setPointCount(vertices);
		circle.setFillColor(fill);
		circle.setOutlineColor(outline);
		circle.setOutlineThickness(thickness);
		circle.setOrigin(radius, radius);
	}
};

class CCollision
{
public:
	float collisionRadius;
	CCollision(float r) 
		: collisionRadius(r) {};
};

class CScore
{
public: 
	int killScore;
	CScore(int s)
		: killScore(s){}
};

class CLifeSpan
{
public:
	int remaining;
	int total;

	CLifeSpan(int t)
		:total(t), remaining(t) {};
};

class CInput // will be used just for Player for now (prolly).
{
public:
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool shoot = false;
	bool specialPower = false;

	CInput(){}
};