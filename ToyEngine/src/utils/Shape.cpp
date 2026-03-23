#include "Shape.h"



void CircleShape::Render(Ref<sf::RenderWindow> window) const
{
	window->draw(m_shape);
}


void RectShape::Render(Ref<sf::RenderWindow> window) const
{
	window->draw(m_shape);
}



void PolygonShape::Render(Ref<sf::RenderWindow> window) const
{

}

