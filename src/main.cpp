#include <iostream>
#include "Game/Game.h"

int main(int argc, char *argv[])
{
    std::unique_ptr<Game> game = std::make_unique<Game>();

    game->Initialize();
    game->Run();
    game->Destroy();

    return 0;
}