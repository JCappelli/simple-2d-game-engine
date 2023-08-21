#ifndef SCRIPTEXECUTIONSYSTEM_H
#define SCRIPTEXECUTIONSYSTEM_H

#include "../Components/ScriptComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../ECS/ECS.h"
#include "../Events/CollisionEvent.h"
#include "../Debugging/Logger.h"

#include <sol/sol.hpp>
#include <glm/glm.hpp>

std::tuple<double, double> GetEntityVelocity(Entity entity)
{
    if (entity.HasComponent<RigidbodyComponent>())
    {
        const auto rigidbody = entity.GetComponent<RigidbodyComponent>();

        return std::make_tuple(rigidbody.velocity.x, rigidbody.velocity.y);
    }
    return std::make_tuple(0.0, 0.0);
}

void SetEntityVelocity(Entity entity, double x, double y)
{
    if (entity.HasComponent<RigidbodyComponent>())
    {
        auto& rigidbody = entity.GetComponent<RigidbodyComponent>();
        rigidbody.velocity = glm::vec2(x, y);
    }
}

class ScriptExecutionSystem: public System
{
private:
    void OnCollisionHappened(CollisionEvent& collisionEvent);

public:
    ScriptExecutionSystem();

    void CreateLuaBindings(sol::state& lua);
    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus);
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
    lua.set_function("get_velocity", GetEntityVelocity);
}

void ScriptExecutionSystem::SubscribeToEvents(std::unique_ptr<EventBus> &eventBus)
{
    eventBus->SubscribeToEvent<CollisionEvent>(this, &ScriptExecutionSystem::OnCollisionHappened);
}

void ScriptExecutionSystem::OnCollisionHappened(CollisionEvent& collisionEvent)
{
    Entity movingBody = collisionEvent.collisionData.movingBody;
    if (movingBody.HasComponent<ScriptComponent>())
    {
        auto scriptComponent = movingBody.GetComponent<ScriptComponent>();
        if (scriptComponent.onHitFunction != sol::lua_nil)
        {
            scriptComponent.onHitFunction.call(collisionEvent.collisionData.movingBody, collisionEvent.collisionData.collidedBody);
        }
    }
}

void ScriptExecutionSystem::Update(float deltaTime)
{
    for(auto entity: GetSystemEntities())
    {
        auto& scriptComponent = entity.GetComponent<ScriptComponent>();
        if (scriptComponent.updateFunction != sol::lua_nil)
        {
            scriptComponent.updateFunction.call(entity, deltaTime);
        }
    }
}

#endif