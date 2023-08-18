#ifndef SCRIPTEXECUTIONSYSTEM_H
#define SCRIPTEXECUTIONSYSTEM_H

#include "../Components/ScriptComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../ECS/ECS.h"
#include "../Debugging/Logger.h"

#include <sol/sol.hpp>
#include <glm/glm.hpp>

void SetEntityVelocity(Entity entity, double x, double y)
{
    Logger::Log("Set Velocity Called!");

    if (entity.HasComponent<RigidbodyComponent>())
    {
        auto& rigidbody = entity.GetComponent<RigidbodyComponent>();
        rigidbody.velocity = glm::vec2(x, y);

        Logger::Log("Setting Velocity to " + std::to_string(x) + " , " + std::to_string(y));
    }
}

class ScriptExecutionSystem: public System
{
public:
    ScriptExecutionSystem();
    void CreateLuaBindings(sol::state& lua);
    void Update(float deltaTime);
};

ScriptExecutionSystem::ScriptExecutionSystem()
{
    RequireComponent<ScriptComponent>();
}

void ScriptExecutionSystem::CreateLuaBindings(sol::state& lua)
{
    lua.new_usertype<EntityFlags>(
        "entity_flags"
    );

    lua.new_usertype<Entity>(
        "entity",
        "get_id", &Entity::GetId,
        "kill", &Entity::Kill, 
        "add_flags", &Entity::AddFlags,
        "remove_flags", &Entity::RemoveFlags,
        "clear_flags", &Entity::ClearFlags,
        "has_flags", &Entity::HasFlags);

    lua.set_function("set_velocity", SetEntityVelocity);
}

void ScriptExecutionSystem::Update(float deltaTime)
{
    for(auto entity: GetSystemEntities())
    {
        auto& scriptComponent = entity.GetComponent<ScriptComponent>();
        scriptComponent.functionality.call(entity, deltaTime);
    }
}

#endif