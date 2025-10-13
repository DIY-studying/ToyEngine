#include <iostream>
#include <memory>
#include <fstream>

#include "SFML/Graphics.hpp"
#include "A2/Game.h"

int main(int argc, char* argv[])
{
    Game game("resource/A2/config.txt");
    game.run();
    return 0;
}