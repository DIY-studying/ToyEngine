#pragma once

#include "SFML/Graphics.hpp"
#include "memory.h"
#include <SFML/System/Vector2.hpp>

class Shape
{
public:
	enum class TypeName
	{
		Circle,
		Rectangle
	};

public:
	Shape(const sf::Vector2f& speed,const sf::Font& font, std::shared_ptr<sf::Shape> shape, TypeName type);
	
	void update();
	void SetPosition(const sf::Vector2f& pos);
	void SetName(const std::string& name);
	void SetNameSize(size_t size);
	void SetNameFillColor(const sf::Vector3i& color);
	void SetShapeFillColor(const sf::Vector3i& color);
	void Draw(sf::RenderWindow& window);
private:
	void CheckBounce(const sf::RenderWindow& window);
private:
	TypeName m_type;
	sf::Vector2f m_speed;
	sf::Text m_text;
	std::shared_ptr<sf::Shape> m_shape;
};

