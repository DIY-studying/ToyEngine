#ifndef  DRAW_H
#define DRAW_H

#include "SFML/Graphics/RenderWindow.hpp"
#include "core.h"

class Render
{
public:
	explicit Render(Ref<sf::RenderWindow> window) : m_window(window) { };
	void drawLine(const vec2& p1, const vec2& p2 , float thick=1.0f , const sf::Color& color = sf::Color::Green);

private:
	Ref<sf::RenderWindow> m_window;
};


#endif // ! DRAW_H

