#ifndef SHAPE_H
#define SHAPE_H

#include "SFML/Graphics.hpp"
#include "core.h"
#include <vector>

#define SHAPE_OVERRIDE_PUB	void Render(Ref<sf::RenderWindow> window) const override;\
												
												


class Shape
{
public:
	virtual ~Shape() = 0;


	virtual void Render(Ref<sf::RenderWindow> window) const = 0;
};

class CircleShape : public Shape
{
public:
	CircleShape(float radius, size_t pointCount=30Ui64) :m_shape(radius,pointCount) { };
	virtual ~CircleShape() { };

	SHAPE_OVERRIDE_PUB

private:
	sf::CircleShape m_shape;
};

class RectShape : public Shape
{
public:
	RectShape(const vec2& size) : m_shape(sf::Vector2f(size.x,size.y)) {};

	SHAPE_OVERRIDE_PUB

private:
	sf::RectangleShape m_shape;
};


class PolygonShape : public Shape
{
public:
	PolygonShape(const std::initializer_list<vec2>& points,const sf::Color& lineColor=sf::Color::Green) :m_points(points),m_LineColor(lineColor) { };

	SHAPE_OVERRIDE_PUB
	

private:
	sf::Color m_LineColor;
	std::vector<vec2> m_points;
};



#endif // !SHAPE_H

