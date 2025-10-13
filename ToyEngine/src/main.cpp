#include <iostream>
#include <memory>
#include <fstream>

#include "SFML/Graphics.hpp"
#include "Config.h"

int main(int argc, char* argv[])
{
    Config config("resource/A1/config.txt");
    sf::RenderWindow& window = config.GetWindow();
    window.setFramerateLimit(60); 
    std::vector<Shape>& shapes = config.GetShapes();

    while (window.isOpen())
    {
        window.clear();    
        for (auto& shape : shapes)
        {
            shape.update();
            shape.Draw(window);
        }
        window.display();               
    }

    return 0;
}