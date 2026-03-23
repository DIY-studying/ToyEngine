#include "Shape.h"


Shape::Shape(const sf::Vector2f& speed, const sf::Font& font, std::shared_ptr<sf::Shape> shape, TypeName type)
	:m_speed(speed),m_text(font),m_shape(shape),m_type(type)
{

}

void Shape::update()
{
	sf::Vector2f pos = m_shape->getPosition() + m_speed;
	SetPosition(pos);
}

void Shape::SetPosition(const sf::Vector2f& pos)
{
	m_shape->setPosition(pos);
	sf::Vector2f localbound = m_shape->getLocalBounds().size;
	localbound.x /= 2;
	localbound.y /= 2;
	sf::Vector2f textlocalbound = m_text.getLocalBounds().size;
	textlocalbound.x /= 2;
	textlocalbound.y /= 2;
	m_text.setPosition(pos+localbound-textlocalbound);
}

void Shape::SetName(const std::string& name)
{
	m_text.setString(name);
}

void Shape::SetNameSize(size_t size)
{
	m_text.setCharacterSize((unsigned int)size);
}


void Shape::SetNameFillColor(const sf::Vector3i& color)
{
	m_text.setFillColor(sf::Color(color.x,color.y,color.z,255));
}

void Shape::SetShapeFillColor(const sf::Vector3i& color)
{
	m_shape->setFillColor(sf::Color(color.x, color.y, color.z, 255));
}

void Shape::Draw(sf::RenderWindow& window)
{
	window.draw(*m_shape);
	window.draw(m_text);
	CheckBounce(window);
}

// 忽略物体超过屏幕大小的情况，即物体上下左右两边同时碰撞
void Shape::CheckBounce(const sf::RenderWindow& window)
{
	sf::Vector2f windowsize =sf::Vector2f(window.getSize().x, window.getSize().y);
	sf::Vector2f localbound = m_shape->getLocalBounds().size;
	
	sf::Vector2f rightup = m_shape->getPosition();
	sf::Vector2f leftdown = -(rightup + localbound-windowsize);

	sf::Vector2f offset = rightup.x <= 0 || rightup.y <= 0 ? rightup : leftdown;

	if (offset.x<=0|| offset.y<=0)
	{
		if (offset.x < offset.y)
			m_speed.x = -m_speed.x;
		else
			m_speed.y = -m_speed.y;
	}
}



