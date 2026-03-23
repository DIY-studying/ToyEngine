#pragma once

#include <string>
#include "EntityManager.h"

template <typename T>
using Ref = std::shared_ptr<T>;

struct PlayerConfig {
    int SR, CR, FR, FG, FB, OR, OG, OB, OT, V;
    float S;
};
struct EnemyConfig {
    int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI;
    float SMIN, SMAX;
};
struct BulletConfig {
    int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L;
    float S;
};


class Game
{
    sf::RenderWindow m_window;              // the window we will draw to
    EntityManager m_entities;                   // vector of entities to maintain
    sf::Font m_font;                                     // the font we will use to draw
    sf::Text m_text;                                    // the score text to be drawn to the screen
    PlayerConfig m_playerConfig;
    EnemyConfig m_enemyConfig;
    BulletConfig m_bulletConfig;
    sf::Clock m_deltaClock;
    int m_score = 0;
    int m_currentFrame = 0;
    std::shared_ptr<Entity> m_player;
    bool m_pause = false;
    bool m_running = true;
    int m_lastEnemySpawnTime=0;
    const int m_bulletspawinterval =15;

    void init(const std::string& path);

    void sMovement();

    void sSteering();

    void sLifespan();

    void sUserInput();

    void sRender();

    void sEnemySpawner();

    void sCollision();

    void sGui();

    void close();

public:
    Game(const std::string& config);

    void run();

    void setPaused(bool paused);

    void spawnPlayer();

    void spawnEnemy();

    void spawnSmallEnemies(std::shared_ptr<Entity> e);

    void spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target);

    void spawnSpecialWeapon(std::shared_ptr<Entity> entity);
};

