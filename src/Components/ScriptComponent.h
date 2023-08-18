#ifndef SCRIPTCOMPONENT_H
#define SCRIPTCOMPONENT_H

#include <sol/sol.hpp>

struct ScriptComponent
{
    sol::function functionality;

    ScriptComponent(sol::function functionality = sol::lua_nil)
    {
        this->functionality = functionality;
    }
};

#endif