#include <iostream>
#include <sol/sol.hpp>

#include "Game/Game.h"

void TestLua()
{
    sol::state lua;
    lua.open_libraries(sol::lib::base);

    lua.script_file("./assets/scripts/test.lua");

    int luaVariable = lua["Basic_variable"];
    std::cout << "The Value is " << luaVariable << std::endl;
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