#ifndef SCRIPTCOMPONENT_H
#define SCRIPTCOMPONENT_H

#include <sol/sol.hpp>

struct ScriptComponent
{
    sol::function updateFunction;
    sol::function onHitFunction;

    ScriptComponent(sol::function updateFunction = sol::lua_nil, sol::function onHitFunction = sol::lua_nil)
    {
        this->updateFunction = updateFunction;
        this->onHitFunction = onHitFunction;
    }
};

#endif