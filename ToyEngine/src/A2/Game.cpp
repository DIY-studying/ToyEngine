#include "Game.h"
#include <fstream>
#include <iostream>
#include "Components.h"
#include <ctime>

#include "imgui-SFML.h"
#include "imgui.h"
#include <sstream>

static int Random(int min, int max)
{
	int diff = max - min + 1;
	int interval = rand() % diff;
	return min + interval;
}

static float Random(float min, float max)
{
	float percent=(float)rand()/RAND_MAX;
	return min + (max - min) * percent;
}


Game::Game(const std::string& config)
	:m_text(m_font)
{
	srand(std::time(0));
	init(config);
	if (!ImGui::SFML::Init(m_window))
		std::cout << "imgui init fail.\n";
}

void Game::run()
{
	spawnPlayer();
	m_deltaClock.start();

	while (m_running)
	{
		m_entities.update();

		sEnemySpawner();
		sLifespan();
		sUserInput();
		sSteering();
		sCollision();
		sMovement();
		sGui();
		sRender();
		

		ImGui::SFML::Render(m_window);
		m_window.display();
		
		close();
	}
}

void Game::setPaused(bool paused)
{
	m_pause = true;
}

void Game::spawnPlayer()
{
	m_player = m_entities.addEntity("player");

	sf::Vector2u size = m_window.getSize();
	Vec2 p(size.x/2.0f,size.y/2.0f);

	Ref<CTransform> transform = std::make_shared<CTransform>(p,Vec2::Zero(), m_playerConfig.S, 0);
	Ref<CShape> shape = std::make_shared<CShape>(m_playerConfig.SR,m_playerConfig.V,
		sf::Color(m_playerConfig.FR,m_playerConfig.FG,m_playerConfig.FB),
		sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB),m_playerConfig.OT);
	Ref<CCollision> collision = std::make_shared<CCollision>(m_playerConfig.CR);
	Ref<CInput> input= std::make_shared<CInput>();

	m_player->cCollision = collision;
	m_player->cTransform = transform;
	m_player->cShape = shape;
	m_player->cInput = input;
}

void Game::spawnEnemy()
{
	Ref<Entity> enemy = m_entities.addEntity("enemy");

	sf::Vector2u size = m_window.getSize();
	float speed = Random(m_enemyConfig.SMIN,m_enemyConfig.SMAX);
	float x = Random((float)m_enemyConfig.SR,(float)size.x- m_enemyConfig.SR);
	float y= Random((float)m_enemyConfig.SR, (float)size.y - m_enemyConfig.SR);
	float angle= Random(0.0f, 360.0f);

	int r = Random(0, 255);
	int g = Random(0, 255);
	int b = Random(0, 255);

	int v = Random(m_enemyConfig.VMIN, m_enemyConfig.VMAX);

	Ref<CTransform> transform = std::make_shared<CTransform>(Vec2(x,y), Vec2::angleToUint(angle) , speed, 0);
	Ref<CShape> shape = std::make_shared<CShape>(m_enemyConfig.SR, v,
		sf::Color(r, g,b),
		sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB), m_enemyConfig.OT);
	Ref<CCollision> collision = std::make_shared<CCollision>(m_enemyConfig.CR);
	Ref<CScore> score= std::make_shared<CScore>(m_enemyConfig.CR);

	enemy->cCollision = collision;
	enemy->cTransform = transform;
	enemy->cShape = shape;

}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{

	sf::CircleShape& circle = e->cShape->circle;
	int vn = circle.getPointCount();
	float angle = (float)360 / vn;
	for (int i = 0; i < vn; i++)
	{
		Ref<Entity> enemy = m_entities.addEntity("smallenemy");
		Vec2 dir = Vec2::angleToUint(angle*i);


		Ref<CTransform> transform = std::make_shared<CTransform>(e->cTransform->pos, dir, e->cTransform->speed, 0);
		Ref<CShape> shape = std::make_shared<CShape>(circle.getRadius()/2, vn, circle.getFillColor(), circle.getOutlineColor(), m_enemyConfig.OT);
		Ref<CCollision> collision = std::make_shared<CCollision>(circle.getOutlineThickness()/2);

		Ref<CLifespan> lifespan= std::make_shared<CLifespan>(m_enemyConfig.L);


		enemy->cCollision = collision;
		enemy->cTransform = transform;
		enemy->cShape = shape;
		enemy->cLifespan = lifespan;
	}

}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target)
{
	Ref<Entity> bullet = m_entities.addEntity("bullet");

	Vec2 pos = entity->cTransform->pos;
	Vec2 dir = target - pos;
	
	Ref<CTransform> transform = std::make_shared<CTransform>(pos, dir, m_bulletConfig.S, 0);
	Ref<CShape> shape = std::make_shared<CShape>(m_bulletConfig.SR, m_bulletConfig.V,
		sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
		sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);
	Ref<CCollision> collision = std::make_shared<CCollision>(m_bulletConfig.CR);
	Ref<CLifespan> lifespan = std::make_shared<CLifespan>(m_bulletConfig.L);

	bullet->cCollision = collision;
	bullet->cTransform = transform;
	bullet->cShape = shape;
	bullet->cLifespan = lifespan;
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
	spawnSmallEnemies(entity);
}

void Game::init(const std::string& path)
{
	std::ifstream fin(path);
	std::string header;
	while (fin>>header)
	{
		if (header == "Window")
		{
			int width, height, framelimit, full;

			fin >> width >> height >> framelimit >> full;
			m_window = sf::RenderWindow(sf::VideoMode(sf::Vector2u(width, height)), "ToyEngine",full?sf::State::Fullscreen:sf::State::Windowed);
			m_window.setFramerateLimit(framelimit);
			
		}
		else if (header == "Font")
		{
			std::string path;
			int fontsize, r, g, b;
			fin >> path >> fontsize >> r >> g >> b;

			if (!m_font.openFromFile(path))
			{
				std::cerr << " path : \"" << path << "\" not found.\n";
			}
			m_text.setCharacterSize(fontsize);
			m_text.setFillColor(sf::Color(r, g, b));
		}
		else if (header == "Player")
		{
			fin >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S >> m_playerConfig.FR >> m_playerConfig.FG >> m_playerConfig.FB
				>> m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB >> m_playerConfig.OT
				 >> m_playerConfig.V;
		}
		else if (header == "Enemy")
		{
			fin>> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.SMIN 
				>> m_enemyConfig.SMAX 
				>> m_enemyConfig.OR
				>> m_enemyConfig.OG >> m_enemyConfig.OB >> m_enemyConfig.OT
				>> m_enemyConfig.VMIN 
				>> m_enemyConfig.VMAX>> m_enemyConfig.L >> m_enemyConfig.SI;
		}
		else if (header == "Bullet")
		{
			fin>> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S >> m_bulletConfig.FR >> m_bulletConfig.FG >> m_bulletConfig.FB
				>> m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB >> m_bulletConfig.OT
				 >> m_bulletConfig.V >> m_bulletConfig.L;
		}
		else
			std::cerr <<"\"" << header <<"\"" << "incorrect header.\n";
	}
}

void Game::sMovement()
{
	for (auto& e : m_entities.getEntities())
	{
		if (e->cTransform)
		{
			Ref<CTransform> transform = e->cTransform;
			transform->pos += transform->direction * transform->speed;
		}
	}
	
}

void Game::sSteering()
{
	for (auto& e : m_entities.getEntities())
	{
		Ref<CTransform> transform = e->cTransform;
		Ref<CInput> input = e->cInput;
		if (transform && input)
		{
			float x = 0, y = 0;
			if (input->left || input->right)
			{
				x = input->left ? -1 : 1;
			}
			if (input->up || input->down)
			{
				y = input->up ? -1 : 1;
			}
			Vec2 velocity(x, y);
			transform->direction = velocity.normalize();
		}
	}
}

void Game::sLifespan()
{
	for (auto& e : m_entities.getEntities())
	{
		if (e->cLifespan && e->cShape)
		{
			e->cLifespan->remaining--;
			if (e->cLifespan->remaining <= 0)
				e->destroy();
			else
			{
				float per = (float)e->cLifespan->remaining / e->cLifespan->total;
				uint8_t alpha = per * 255;
				sf::CircleShape& circle = e->cShape->circle;
				sf::Color fillColor = circle.getFillColor();
				sf::Color outColor = circle.getOutlineColor();
				circle.setFillColor({ fillColor.r ,fillColor.g,fillColor.b,alpha });
				circle.setOutlineColor({ outColor.r ,outColor.g,outColor.b,alpha });
			}
		}
	}
}

void Game::sUserInput()
{
	
	static int counter = m_bulletspawinterval;
	counter++;

	bool up=false, down = false, left = false, right = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		up = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		down = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		left = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		right = true;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		if (counter >= m_bulletspawinterval)
		{
			counter = 0;
			spawnBullet(m_player, (Vec2)sf::Mouse::getPosition());
		}
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
		spawnSpecialWeapon(m_player);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
		m_pause = true;

	while (const std::optional event = m_window.pollEvent())
	{
		ImGui::SFML::ProcessEvent(m_window, *event);
		if (event->is<sf::Event::Closed>())
		{
			
			m_running = false;
		}
			
	}
	for (auto& e : m_entities.getEntities())
	{
		if (e->cInput)
		{
			Ref<CInput> input = e->cInput;
			
			input->up = up;
			input->down = down;
			input->left = left;
			input->right = right;
		}
	}
}

void Game::sRender()
{
	m_window.clear(sf::Color(18,33,43));
	for (auto e : m_entities.getEntities())
	{
		if (e->cShape && e->cTransform)
		{
			sf::CircleShape& c = e->cShape->circle;
			Ref<CTransform> transform = e->cTransform;
			c.setPosition({ transform->pos.x,transform->pos.y });
			c.rotate(sf::degrees(transform->angle));
			m_window.draw(c);
		}
	}
}

void Game::sEnemySpawner()
{
	m_lastEnemySpawnTime++;
	if (m_lastEnemySpawnTime >= m_enemyConfig.SI)
	{
		m_lastEnemySpawnTime = 0;
		spawnEnemy();
	}
}

void Game::close()
{
	if (!m_running)
	{
		m_window.close();
		ImGui::SFML::Shutdown();
	}
}

/// <summary>
/// 检测物体是否碰到窗口的边界，改变物体速度方向。
/// </summary>
/// <param name="velocity">速度方向</param>
/// <param name="pos">物体位置</param>
/// <param name="localBound">物体碰撞体积</param>
/// <param name="windowSize">窗口大小</param>
static void CheckBounce(Vec2& velocity,const Vec2& pos, const Vec2& localBound, const Vec2& windowSize)
{
	Vec2 rightup = pos - localBound;
	Vec2 leftdown = (pos + localBound)- windowSize;

	if ((rightup.x <= 0 && velocity.x<0 )||(leftdown.x>=0 && velocity.x>0))
	{
		velocity.x = -velocity.x;
	}
	if ((rightup.y <= 0 && velocity.y < 0) || (leftdown.y >= 0 && velocity.y > 0))
	{
		velocity.y = -velocity.y;
	}
}

void Game::sCollision()
{
	for (auto& e : m_entities.getEntities())
	{
		if(e->cTransform && e->cCollision)
			CheckBounce(e->cTransform->direction,e->cTransform->pos, Vec2(e->cCollision->radius), (Vec2)m_window.getSize());
	}

	for (auto& e : m_entities.getEntities("bullet"))
	{
		if (e->isActive())
		{
			for (auto& y : m_entities.getEntities("enemy"))
			{
				if (e->cTransform->pos.dist(y->cTransform->pos) <= e->cShape->circle.getRadius() + y->cShape->circle.getRadius())
				{
					e->destroy();
					y->destroy();
					spawnSmallEnemies(y);
				}
			}
		}
	}
}

void Game::sGui()
{
	ImGui::SFML::Update(m_window, m_deltaClock.restart());

	ImGui::Begin("Hello, world!");
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Entity id: tag active");
	ImGui::BeginChild("Scrolling");
	int i = 0;
	for (auto& e : m_entities.getEntities())
	{
		std::stringstream ss;
		ss << e->id() << ": " << e->tag()<<" " << (e->isActive() ? "True":"false");
		ImGui::Text(ss.str().c_str(), i++);
	}
		
	ImGui::EndChild();
	ImGui::End();
}
