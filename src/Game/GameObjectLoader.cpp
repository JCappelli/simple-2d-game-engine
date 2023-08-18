#include "GameObjectLoader.h"

#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/PlayerMovementComponent.h"
#include "../Components/PlayerShootingComponent.h"
#include "../Components/ScriptComponent.h"
#include "../Components/HealthComponent.h"
#include <glm/glm.hpp>

void GameObjectLoader::LoadPlayerEntity(const int x, const int y, const std::unique_ptr<Registry> &registry)
{
    Entity player = registry->CreateEntity();
    player.AddFlags(EntityFlags::Player);
    player.AddComponent<TransformComponent>( 
        glm::vec2(x, y), 
        glm::vec2(1,1),
        0.0);
    player.AddComponent<RigidbodyComponent>(
        glm::vec2(20,20));
    player.AddComponent<PlayerMovementComponent>(
        75);
    player.AddComponent<PlayerShootingComponent>(
        5*16,
        8*16,
        "tilemap",
        160,
        200, 
        5);
    player.AddComponent<SpriteComponent>(
        16,
        16,
        0*16,
        7*16,
        2,
        "tilemap");
    player.AddComponent<AnimationComponent>(
        2,
        5,
        true);
    player.AddComponent<BoxColliderComponent>(
        16,
        16,
        0,
        0);
    player.AddComponent<CameraFollowComponent>();
}

void GameObjectLoader::LoadEnemyEntity(const int x, const int y, const std::unique_ptr<Registry> &registry, sol::state& lua)
{
    Entity enemy = registry->CreateEntity();
    enemy.AddFlags(EntityFlags::Enemy);
    enemy.AddComponent<TransformComponent>(
        glm::vec2(x, y),
        glm::vec2(1,1),
        0.0);
    enemy.AddComponent<SpriteComponent>(
        16,
        16,
        1 * 16,
        9 * 16,
        2,
        "tilemap");
    enemy.AddComponent<HealthComponent>(10);
    enemy.AddComponent<BoxColliderComponent>(
        16,
        16,
        0,
        0);

    //Load Enemy Script
    sol::load_result enemyScript = lua.load_file("./assets/scripts/enemyscript.lua");

    if (enemyScript.valid() == false)
    {
        sol::error luaError = enemyScript;
        std::string errorMessage = luaError.what();

        Logger::Log("Lua Error on Enemy Script Load: " + errorMessage);
        return;
    }
    enemyScript.call();

    sol::optional<sol::function> hasUpdateFunction = lua["on_update"];
    if (hasUpdateFunction != sol::nullopt)
    {
        sol::function updateFunction = lua["on_update"];
        enemy.AddComponent<ScriptComponent>(updateFunction);
    }
    else
    {
        Logger::Log("No Update Function in Enemy Script!");
    }
}

void GameObjectLoader::LoadCollisionEntity(const int x, const int y, const int width, const int height,  const std::unique_ptr<Registry> &registry)
{
    Entity collisionEntity = registry->CreateEntity();
    collisionEntity.AddComponent<TransformComponent>(
        glm::vec2(x, y),
        glm::vec2(1,1),
        0.0);
    collisionEntity.AddComponent<BoxColliderComponent>(
        height,
        width,
        0,
        0);
}
