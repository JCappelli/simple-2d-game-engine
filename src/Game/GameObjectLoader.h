#ifndef GAMEOBJECTLOADER_H
#define GAMEOBJECTLOADER_H

#include "../ECS/ECS.h"
#include <sol.hpp>

class GameObjectLoader
{
public:
    static void LoadPlayerEntity(const int x, const int y, const std::unique_ptr<Registry>& registry);
    static void LoadEnemyEntity(const int x, const int y, const std::unique_ptr<Registry>& registry, sol::state& lua);
    static void LoadCollisionEntity(const int x, const int y, const int width, const int height,  const std::unique_ptr<Registry> &registry);
};

#endif