#ifndef DEBUGSCENE_H
#define DEBUGSCENE_H

#include "scene/Scene.h"
#include "GameEngine.h"

#include "core.h"

#include "imgui-SFML.h"
#include "imgui.h"

#include "Render/Render.h"
#include <iostream>

class DebugScene : public Scene
{
public:

	explicit DebugScene(GameEngine* game):Scene(game),m_render(game->window()),thick(1), m_radiu(6), p2(600, 400)
		,outlineThick(2)
	{
		m_deltaClock.start();
	};

	virtual ~DebugScene()
	{

	};

	

protected:

	void update() override;

	void onEnd() override;

	void sDoAction(const Action& action) override;

	void sRender() override;

private:
	void sGui();

private:
	sf::Clock m_deltaClock;
	Render m_render;
	vec2 p1, p2;
	float m_radiu,thick,outlineThick;
	float m_angle=0;
	vec2 m_ori,c_ori;
};


inline void DebugScene::update()
{
	sRender();
	sGui();
}

inline void DebugScene::onEnd()
{

}

inline void DebugScene::sGui()
{
	ImGui::SFML::Update(*m_game->window(), m_deltaClock.restart());
	ImGui::Begin("Hello, world!");

	ImGui::Separator();
	ImGui::DragFloat("radiu", &m_radiu, 0.01f, 0.0f, 1280.0f);
	ImGui::DragFloat("outthick", &outlineThick, 0.01f, 0.0f, 1280.0f);
	ImGui::DragFloat("angle", &m_angle, 0.01f, 0.0f, 1280.0f);
	
	ImGui::Separator();
	ImGui::DragFloat("ori:x", &m_ori.x, 1.0f, -1280.0f, 1280.0f);
	ImGui::DragFloat("ori:y", &m_ori.y, 1.0f, -1280.0f, 768.0f);
	ImGui::DragFloat("cori:x", &c_ori.x, 1.0f, -1280.0f, 1280.0f);
	ImGui::DragFloat("cori:y", &c_ori.y, 1.0f, -1280.0f, 768.0f);

	ImGui::Separator();
	ImGui::DragFloat("p1:x", &p1.x, 1.0f, 0.0f, 1280.0f);
	ImGui::DragFloat("p1:y", &p1.y, 1.0f, 0.0f, 768.0f);

	ImGui::Separator();
	ImGui::DragFloat("p2:x", &p2.x, 1.0f, 0.0f, 1280.0f);
	ImGui::DragFloat("p2:y", &p2.y, 1.0f, 0.0f, 768.0f);

	ImGui::Separator();
	ImGui::DragFloat("thick", &thick, 1.0f, 0.0f, 1280.0f);
	
	ImGui::End();
	
}


inline void DebugScene::sDoAction(const Action& action)
{

}

inline void DebugScene::sRender()
{
	m_game->window()->clear(sf::Color::Black);
	m_render.drawLine(p1,p2);
	sf::CircleShape c1(m_radiu), c2(m_radiu);
	sf::Vector2f ori(m_radiu/2.0f, m_radiu/ 2.0f);
	c1.setOrigin(ori);
	c2.setOrigin(ori);
	
	c1.setPosition(sf::Vector2f(p1.x, p1.y));
	c2.setPosition(sf::Vector2f(p2.x, p2.y));

	c1.setOutlineColor(sf::Color::Green);
	c2.setOutlineColor(sf::Color::Green);
	c1.setOutlineThickness(outlineThick);
	c2.setOutlineThickness(outlineThick);

	c1.setFillColor(sf::Color::Transparent);
	c2.setFillColor(sf::Color::Transparent);

	float length = (sf::Vector2f(p1.x, p1.y) - sf::Vector2f(p2.x, p2.y)).length();

	sf::RectangleShape rect(sf::Vector2f(length,thick));
	rect.setOrigin(sf::Vector2f(0,thick/2.0f));
	rect.setPosition(sf::Vector2f(p1.x, p1.y));
	rect.setFillColor(sf::Color::Green);

	vec2 dir = (p2 - p1).normalize();
	float degree = angle_radians(dir, vec2(1, 0));
	if (!clockwise(dir, vec2(1, 0)))
	{
		degree =2*vec2::pi-degree;
	}
	rect.setRotation(sf::radians(degree));

	

	m_game->window()->draw(c2);
	m_game->window()->draw(c1);
	m_game->window()->draw(rect);

}





#endif // !DUBGSCENE_H




