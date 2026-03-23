#include "Config.h"
#include <string>
#include <iostream>
#include "Shape.h"
#include <memory>


Config::Config(const std::string& path)
	:m_if(path),m_Shapes()
{
	std::string name;
	while (m_if >> name)
	{
		Type type = StringToType(name);
		switch (type)
		{
			case Type::Window:
				PharseWidnow();
				break;
			case Type::Font:
				PharseFont();
				break;
			case Type::Circle:
				PharseCircle();
				break;
			case Type::Rectangle:
				PharseRectangle();
				break;
			default:
				m_if.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
				break;
		}
	}
}

Config::Type Config::StringToType(const std::string& name)
{
	if (name == "Window")
		return Type::Window;
	else if (name == "Font")
		return Type::Font;
	else if (name == "Circle")
		return Type::Circle;
	else if (name == "Rectangle")
		return Type::Rectangle;
	else
	{
		std::cerr << '"' << name << '"' << " unknow type!\n";
		return Type::None;
	}
		
}

void Config::PharseWidnow()
{
	sf::Vector2u size;
	m_if >> size.x >> size.y;
	m_window = sf::RenderWindow(sf::VideoMode(size),"ToyEngine");
}

void Config::PharseFont()
{
	std::string fontflie;
	m_if >> fontflie >>m_TextSize>>m_TextColor.x>> m_TextColor.y>> m_TextColor.z;
	if (!m_font.openFromFile(fontflie))
	{
		std::cerr << "file path \"" << fontflie << "\" not find.\n";
	}
}

void Config::PharseCircle()
{
	std::string name;
	sf::Vector2f pos;
	sf::Vector2f speed;
	sf::Vector3i color;
	float radiu;

	m_if >> name >> pos.x >> pos.y >> speed.x >> speed.y >> color.x >> color.y >> color.z>> radiu;

	std::shared_ptr<sf::CircleShape> circle(new sf::CircleShape(radiu));
	Shape shape(speed,m_font,circle,Shape::TypeName::Circle);
	shape.SetPosition(pos);
	shape.SetShapeFillColor(color);
	shape.SetName(name);
	shape.SetNameSize(m_TextSize);
	shape.SetNameFillColor(m_TextColor);
	
	m_Shapes.push_back(shape);
}

void Config::PharseRectangle()
{
	std::string name;
	sf::Vector2f pos;
	sf::Vector2f speed;
	sf::Vector3i color;
	float width, height;

	m_if >> name >> pos.x >> pos.y >> speed.x >> speed.y >> color.x >> color.y >> color.z >> width>>height;

	std::shared_ptr<sf::RectangleShape> rectangle(new sf::RectangleShape({width,height}));
	Shape shape(speed, m_font, rectangle, Shape::TypeName::Circle);
	shape.SetPosition(pos);
	shape.SetShapeFillColor(color);
	shape.SetName(name);
	shape.SetNameSize(m_TextSize);
	shape.SetNameFillColor(m_TextColor);

	m_Shapes.push_back(shape);

}




