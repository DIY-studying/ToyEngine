#include <iostream>
#include <utility>

#include "SFML/Window/Event.hpp"
#include "GameEngine.h"
#include "input/Assets.h"
#include "input/Action.h"

#include "Demo/Scene_Menu.h"
#include "Demo/Scene_Play.h"

#include "Debug/DebugScene.h"

#include "imgui-SFML.h"
#include "imgui.h"


GameEngine::GameEngine(const std::string &path) {
    m_window = make_Ref<sf::RenderWindow>(sf::VideoMode(sf::Vector2u(1280, 768)), "Definitely Not Mario");
    if (!ImGui::SFML::Init(*m_window))
        std::cout << "imgui init fail.\n";
    init(path);
}

GameEngine::GameEngine()
{
   
    m_window = make_Ref<sf::RenderWindow>(sf::VideoMode(sf::Vector2u(1280, 768)), "Definitely Not Mario");
    if (!ImGui::SFML::Init(*m_window))
        std::cout << "imgui init fail.\n";

    changeScene("DEBUG", make_Ref<DebugScene>(this));
}

void GameEngine::init(const std::string &path) {
    m_assets.loadFromFile(path);

    m_window->setFramerateLimit(60);
    m_window->setKeyRepeatEnabled(false);

    changeScene("DEBUG", make_Ref<DebugScene>(this));
}

std::shared_ptr<Scene> GameEngine::currentScene() {
    return m_sceneMap[m_currentScene];
}

bool GameEngine::isRunning() {
    return m_running && m_window->isOpen();
}

Ref<sf::RenderWindow> GameEngine::window() {
    return m_window;
}

void GameEngine::run() {
    while (isRunning()) {
       
        sUserInput();
        update();

        ImGui::SFML::Render(*m_window);
        m_window->display();
    }
}

void GameEngine::sUserInput() {
    while (auto event= m_window->pollEvent()) {
        ImGui::SFML::ProcessEvent(*m_window, *event);
        if (event->is<sf::Event::Closed>()) {
            quit();
        }
        
        const sf::Event::KeyPressed* keyPress = event->getIf<sf::Event::KeyPressed>();
        const sf::Event::KeyReleased* keyRelease = event->getIf<sf::Event::KeyReleased>();
        bool iskeyPressed = event->is<sf::Event::KeyPressed>(), iskeyReleased = event->is<sf::Event::KeyReleased>();
        if (iskeyPressed) {
            if (keyPress->code == sf::Keyboard::Key::X) {
                std::cout << "Save screenshot to " << "test.png" << std::endl;
                sf::Texture texture;
                if (texture.resize(m_window->getSize()))
                    std::cout << "texture resize fail.\n";;
                texture.update(*m_window);
                if (texture.copyToImage().saveToFile("test.png")) {
                    std::cout << "Screenshot saved to " << "test.png" << std::endl;
                }
            }
        }

        if (iskeyPressed || iskeyReleased) {
            int code =int(( keyPress!=nullptr) ? keyPress->code : keyRelease->code);
            // if the current scene does not have an action associated with this key, skip the event
            if (currentScene()->getActionMap().find(code) == currentScene()->getActionMap().end()) {
                continue;
            }

            // determine start or end action by whether it was key press or release
            const std::string actionType = (iskeyPressed) ? "START" : "END";

            

            // look up the action and send the action to the scene
            currentScene()->doAction(Action(currentScene()->getActionMap().at(code), actionType));
        }
    }
}

void GameEngine::changeScene(const std::string &sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene) {
    m_currentScene = sceneName;
    m_sceneMap[sceneName] = scene;
}

void GameEngine::quit() {
    m_running = false;
    m_window->close();
}

void GameEngine::update() {
    currentScene()->update();
}

const Assets &GameEngine::assets() const {
    return m_assets;
}
