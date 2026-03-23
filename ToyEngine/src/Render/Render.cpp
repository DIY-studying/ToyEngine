#include "Render.h"

#include "SFML/Graphics.hpp"


void  Render::drawLine(const vec2& p1, const vec2& p2, float thick,const sf::Color& color)
{
	sf::RectangleShape line(sf::Vector2f((p2-p1).length(), thick));

   m_window->draw(line);
}