#pragma once

#include <string>
#include <fstream>
#include<SFML/Graphics.hpp>
#include <Shape.h>

class Config
{
	enum class Type
	{
		None,
		Window,
		Font,
		Circle,
		Rectangle
	};
public:
	Config(const std::string& path);

	inline sf::RenderWindow& GetWindow() { return m_window;  };
	inline std::vector<Shape>& GetShapes() { return m_Shapes; };

private:
	static Type StringToType(const std::string&);
private:
	void PharseWidnow();
	void PharseFont();
	void PharseCircle();
	void PharseRectangle();
	
private:
	std::ifstream m_if;
	sf::Font  m_font;
	sf::Vector3i m_TextColor;
	size_t m_TextSize;
	sf::RenderWindow m_window;
	std::vector<Shape> m_Shapes;
};

