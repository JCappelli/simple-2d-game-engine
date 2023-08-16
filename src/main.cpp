#include <iostream>
#include <sol/sol.hpp>

#include "Game/Game.h"

void TestLua()
{
    sol::state lua;
    lua.open_libraries(sol::lib::base);

    lua.script_file("./assets/scripts/test.lua");

    int luaVariable = lua["Basic_variable"].get_or(0);
    std::cout << "The Variable is " << luaVariable << std::endl;

    sol::table config = lua["config"];
    
    bool isFullscreen = config["fullscreen"];
    int screenWidth = config["resolution"]["width"];
    int screenHeight = config["resolution"]["height"];

    std::cout << "Fullscreen table value: " << isFullscreen << std::endl;
    

    std::cin.get();
}

int main(int argc, char *argv[])
{
    TestLua();

    // std::unique_ptr<Game> game = std::make_unique<Game>();

    // game->Initialize();
    // game->Run();
    // game->Destroy();

    return 0;
}