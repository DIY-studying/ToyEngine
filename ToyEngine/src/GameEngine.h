#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <map>
#include <memory>

#include "SFML/Graphics/RenderWindow.hpp"
#include "scene/Scene.h"
#include "input/Assets.h"
#include "core.h"


typedef std::map<std::string, Ref<Scene>> SceneMap;

class GameEngine {
protected:
    Ref<sf::RenderWindow> m_window;
    Assets m_assets;
    std::string m_currentScene;
    SceneMap m_sceneMap;
    size_t m_simulationSpeed = 1;
    bool m_running = true;

    void init(const std::string &path);

    void update();

    void sUserInput();


   Ref<Scene> currentScene();

public:
    GameEngine(const std::string &path);
    GameEngine();

    void changeScene(const std::string &sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene = false);

    void quit();

    void run();

    Ref<sf::RenderWindow> window();

    const Assets &assets() const;

    bool isRunning();
 
};

#endif //GAME_ENGINE_H
