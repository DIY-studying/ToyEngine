#include <iostream>
#include <fstream>

#include "SFML//Window/Event.hpp"
#include "Scene_Menu.h"
#include "Scene_Play.h"
#include "input/Assets.h"
#include "system/Physics.h"
#include "GameEngine.h"
#include "Entity/Components.h"
#include "input/Action.h"
#include "SFML/Graphics/RectangleShape.hpp"


Scene_Play::Scene_Play(GameEngine *gameEngine, const std::string &levelPath)
        : Scene(gameEngine), m_levelPath(levelPath) ,m_gridText(m_game->assets().getFont("Mario"),"default") {
    init(levelPath);
}

void Scene_Play::init(const std::string &levelPath) {
    registerAction((int)sf::Keyboard::Key::P, "PAUSE");
    registerAction((int)sf::Keyboard::Key::Escape, "QUIT");
    registerAction((int)sf::Keyboard::Key::T, "TOGGLE_TEXTURE");         // Toggle drawing (T)extures
    registerAction((int)sf::Keyboard::Key::C, "TOGGLE_COLLISION");      // Toggle drawing (C)ollision Boxes
    registerAction((int)sf::Keyboard::Key::G, "TOGGLE_GRID");                 // Toggle drawing (G)rid


    registerAction((int)sf::Keyboard::Key::W, "JUMP");
    registerAction((int)sf::Keyboard::Key::A, "LEFT");
    registerAction((int)sf::Keyboard::Key::D, "RIGHT");
    registerAction((int)sf::Keyboard::Key::S, "SHOOT");
    registerAction((int)sf::Keyboard::Key::L,"Debug");

    m_gridText.setCharacterSize(12);
    m_gridText.setFont(m_game->assets().getFont("Mario"));
    // m_gridText.setFont(m_game->assets().getFont("Tech"));

    loadLevel(levelPath);

    spawnPlayer();
}

vec2 Scene_Play::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity) {

    vec2 size = entity->getComponent<CAnimation>().animation.getSize();
    vec2 windowSize =(vec2) m_game->window()->getSize();
    float x =  (m_gridSize.x * gridX+size.x/2);
    float y = windowSize.y - (m_gridSize.y * gridY + size.y / 2);

    return vec2(x, y);
}

void Scene_Play::loadLevel(const std::string &fileName) {
    m_entityManager = EntityManager();

    std::ifstream file( m_game->assets().m_assertDir + fileName);
    std::string entityType;
    const Assets& assert = m_game->assets();
    while (file>>entityType)
    {
        if(entityType=="Tile"|| entityType == "Dec")
        {
            std::string aniName;
            float xGrid, yGrid;
            file >> aniName >> xGrid >> yGrid;
            std::shared_ptr<Entity> e= m_entityManager.addEntity(aniName);
            Animation ani = assert.getAnimation(aniName);
            e->addComponent<CAnimation>(ani,true);
            vec2 pos = gridToMidPixel(xGrid,yGrid,e);
            e->addComponent<CTransform>(pos);
            if(entityType == "Tile")
                e->addComponent<CBoundingBox>(m_gridSize);
        }
        else if (entityType=="Player")
        {
            file >> m_playerConfig.X>>m_playerConfig.Y>>m_playerConfig.CX>>m_playerConfig.CY
                >>m_playerConfig.SPEED>> m_playerConfig.MAX_SPEED>>m_playerConfig.JUMP>>m_playerConfig.GRAVITY
                >>m_playerConfig.WEAPON>>m_playerConfig.WEAPONLIFE;
        }
    }
}

void Scene_Play::spawnPlayer() {
    m_player = m_entityManager.addEntity("Player");
    m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Stand"), true);
    m_player->addComponent<CTransform>(vec2(224, 352));
    m_player->addComponent<CBoundingBox>(vec2(m_playerConfig.CX,m_playerConfig.CY));

    m_player->addComponent<CState>("air");
    m_player->addComponent<CInput>();
    m_player->addComponent<CGravity>(m_playerConfig.GRAVITY);
}

void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity) {
    std::shared_ptr<Entity> bullet = m_entityManager.addEntity("Bullet");

    CTransform& transform = m_player->getComponent<CTransform>();
    vec2 speed;
    speed.x = transform.scale.x * m_playerConfig.SPEED;
    speed.y = 0;

    const Animation& ani = m_game->assets().getAnimation(m_playerConfig.WEAPON);


    bullet->addComponent<CAnimation>(ani,true);
    bullet->addComponent<CTransform>(transform.pos, speed);
}


void Scene_Play::update() {
    m_entityManager.update();

   
    sMovement();
    sSteering();
    sLifespan();
    sCollision();
    sAnimation();
    sRender();
    m_currentFrame++;
}

void Scene_Play::sSteering()
{
    CGravity& gravity = m_player->getComponent<CGravity>();
    CTransform& transform = m_player->getComponent<CTransform>();
    CState& state = m_player->getComponent<CState>();
    CInput& inpute = m_player->getComponent<CInput>();

    float dir = (transform.velocity.x>0)?1:-1;
    transform.velocity.x = 0;
    if (state.state == "air")
    {
        transform.velocity.y += gravity.gravity;
    }
    else if (state.state == "ground")
    {
        transform.velocity.y = 0;
    }
    if (inpute.up&& (state.state == "ground" || state.state == "run"))
    {
        state.state = "air";
        transform.velocity.y -= m_playerConfig.JUMP;
    }
    if (inpute.left || inpute.right)
    {
        transform.velocity.x = inpute.right ? m_playerConfig.SPEED : -m_playerConfig.SPEED;
    }
    if (transform.velocity.x != 0)
    {
        transform.scale = vec2(dir, 1);
    }

    if (transform.velocity.length() >= m_playerConfig.MAX_SPEED)
    {
        transform.velocity = transform.velocity.normalize() * m_playerConfig.MAX_SPEED;
    }
}

void Scene_Play::sMovement() {
    CTransform& transform = m_player->getComponent<CTransform>();
    transform.prevPos = transform.pos;
    transform.pos += transform.velocity;
    
    for (auto& e : m_entityManager.getEntities("Bullet"))
    {
        CTransform& t = e->getComponent<CTransform>();
        t.prevPos = t.pos;
        t.pos += t.velocity;
    }
}

void Scene_Play::sLifespan() {
 
}

void Scene_Play::sCollision()
{
   

    CState& state = m_player->getComponent<CState>();
    CTransform& transform = m_player->getComponent<CTransform>();
    state.state = "air";
    for (auto& e : m_entityManager.getEntities())
    {
        if (e->tag() == "Brick" || e->tag() == "QuestionHit" || e->tag() == "Question" || e->tag() == "Ground")
        {
            CTransform& t = e->getComponent<CTransform>();
            vec2 size = m_worldPhysics.GetOverlap(m_player, e);
            if (size.x != 0 && size.y != 0)
            {
                vec2 preSize = m_worldPhysics.GetPreviousOverlap(m_player, e);
                if (!preSize)
                {
                    if (preSize.y > preSize.x)
                    {
                        if (t.pos.x > transform.pos.x)
                        {
                            transform.pos.x -= size.x;
                        }
                        else
                        {
                            transform.pos.x += size.x;
                        }
                    }
                    else
                    {
                        if (t.pos.y > transform.pos.y)
                        {
                            if (transform.velocity.x != 0)
                            {
                                state.state = "run";
                            }
                            else
                            {
                                state.state = "ground";
                            }
                            transform.pos.y -= (size.y - vec2::epsilon);
                        }
                        else
                        {
                            const Animation& a = m_game->assets().getAnimation("Explosion");
                            e->addComponent<CAnimation>(a, false);
                            transform.pos.y += size.y;
                        }
                    }
                }
            }
        }
    }
}


void Scene_Play::sDoAction(const Action &action) {
    CInput& inpute = m_player->getComponent<CInput>();
    if (action.type() == "START") {
        if (action.name() == "TOGGLE_TEXTURE") {m_drawTextures = !m_drawTextures; }
        else if (action.name() == "TOGGLE_COLLISION") { m_drawCollision = !m_drawCollision; }
        else if (action.name() == "TOGGLE_GRID") { m_drawGrid = !m_drawGrid; }
        else if (action.name() == "PAUSE") { setPaused(!m_paused); }
        else if (action.name() == "QUIT") { onEnd(); }
        else if (action.name() == "JUMP") {
            inpute.up = true;
        }
        else if (action.name() == "Debug")
        {
            __debugbreak();
        }
        else if (action.name() == "LEFT") {
            inpute.left = true;
        }
        else if (action.name() == "RIGHT") {
            inpute.right = true;
        }
        else if (action.name() == "SHOOT") {
            spawnBullet(m_player);
        }
    }
    else if (action.type() == "END") {
        if (action.name() == "JUMP") {
            inpute.up = false;
        }
        else if (action.name() == "LEFT") {
            inpute.left = false;
        }
        else if (action.name() == "RIGHT") {
            inpute.right = false;
        }
    }
}

void Scene_Play::sAnimation() {

    CState& state = m_player->getComponent<CState>();
    CAnimation& ani= m_player->getComponent<CAnimation>();
    if (state.state == "air")
    {
        const Animation& a = m_game->assets().getAnimation("Jump");
        if(ani.animation.getName()!=a.getName())
        m_player->addComponent<CAnimation>(a, true);
    }
    else if (state.state == "ground")
    {
        const Animation& a = m_game->assets().getAnimation("Stand");
        if (ani.animation.getName() != a.getName())
        m_player->addComponent<CAnimation>(a, true);
    }
    else if (state.state == "run")
    {
        const Animation& a = m_game->assets().getAnimation("Run");
        if (ani.animation.getName() != a.getName())
        m_player->addComponent<CAnimation>(a, true);
    }

    for (auto& e : m_entityManager.getEntities())
    {
        CAnimation& ani = e->getComponent<CAnimation>();



        if (ani.has)
        {
            if (ani.animation.hasEnded())
            {
                if (ani.repeat)
                {
                    ani.animation.update();
                }
                if (ani.animation.getName() == "Explosion")
                {
                    e->destroy();
                }
            }
            else
            {
                ani.animation.update();

            }

        }


    }
}

void Scene_Play::onEnd() {
    m_game->changeScene( "MENU", std::make_shared<Scene_Menu>(m_game));
}

void Scene_Play::sRender() {
    // color the background darker, so you know that the game is paused
    if (!m_paused) {
        m_game->window()->clear(sf::Color(100, 100, 255));
    } else {
        m_game->window()->clear(sf::Color(50, 50, 150));
    }

    // set the viewport of the window to be centered on the player if it's far enough right
    auto &pPos = m_player->getComponent<CTransform>().pos;
    float windowCenterX = std::max(m_game->window()->getSize().x / 2.0f, pPos.x);
    sf::View view = m_game->window()->getView();
    view.setCenter({ windowCenterX, m_game->window()->getSize().y - view.getCenter().y });
    m_game->window()->setView(view);

    // draw all Entity textures / animations
    if (m_drawTextures) {
        for (const auto &e: m_entityManager.getEntities()) {
            auto &transform = e->getComponent<CTransform>();
            if (e->hasComponent<CAnimation>()) {
                auto &animation = e->getComponent<CAnimation>().animation;
                animation.getSprite().setRotation( sf::radians(transform.angle) );
                animation.getSprite().setPosition(
                    { transform.pos.x, transform.pos.y }
                );
                animation.getSprite().setScale(
                    { transform.scale.x, transform.scale.y }
                );
                m_game->window()->draw(animation.getSprite());
            }
        }
    }

    // draw all Entity collision bounding boxes with a rectangle shape
    if (m_drawCollision) {
        for (const auto &e: m_entityManager.getEntities()) {
            if (e->hasComponent<CBoundingBox>()) {
                auto &box = e->getComponent<CBoundingBox>();
                auto &transform = e->getComponent<CTransform>();
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
                rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
                rect.setPosition({ transform.pos.x, transform.pos.y });
                rect.setFillColor(sf::Color(0, 0, 0, 0));
                rect.setOutlineColor(sf::Color::White);
                rect.setOutlineThickness(1);
                m_game->window()->draw(rect);
            }
        }
    }

    // draw the grid so that can easily debug
    if (m_drawGrid) {
        float leftX = m_game->window()->getView().getCenter().x - width() / 2.0;
        float rightX = leftX + width() + m_gridSize.x;
        float nextGridX = leftX - ((int) leftX % (int) m_gridSize.x);

        for (float x = nextGridX; x < rightX; x += m_gridSize.x) {
            drawLine(vec2(x, 0), vec2(x, height()));
        }

        for (float y = 0; y < height(); y += m_gridSize.y) {
            drawLine(vec2(leftX, height() - y), vec2(rightX, height() - y));

            for (float x = nextGridX; x < rightX; x += m_gridSize.x) {
                std::string xCell = std::to_string((int) x / (int) m_gridSize.x);
                std::string yCell = std::to_string((int) y / (int) m_gridSize.y);
                m_gridText.setString("(" + xCell + "," + yCell + ")");
                m_gridText.setPosition({ x + 3, height() - y - m_gridSize.y + 2 });
                m_game->window()->draw(m_gridText);
            }
        }
    }
}
