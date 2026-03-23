#include <iostream>
#include <memory>
#include <fstream>

#include "SFML/Graphics.hpp"
#include "core.h"
#include "GameEngine.h"

int main(int argc, char* argv[])
{
    GameEngine game;
    game.run();

    return 0;
}