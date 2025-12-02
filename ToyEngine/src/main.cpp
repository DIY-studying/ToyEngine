#include <iostream>
#include <memory>
#include <fstream>

#include "SFML/Graphics.hpp"
#include "A3/GameEngine.h"

int main(int argc, char* argv[])
{
    GameEngine game("resource/A3/assets.txt");
    game.run();
    return 0;
}