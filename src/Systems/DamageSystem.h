#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include "../ECS/ECS.h"
#include "../Events/EventBus.h"
#include "../Events/CollisionEvent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Debugging/Logger.h"

class DamageSystem: public System
{
public:
    DamageSystem();

    void SubscibeToEvents(std::unique_ptr<EventBus>& eventBus);
    void OnCollisionHappened(CollisionEvent& event);
};

DamageSystem::DamageSystem()
{
    RequireComponent<BoxColliderComponent>();
}

void DamageSystem::SubscibeToEvents(std::unique_ptr<EventBus>& eventBus)
{
    eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::OnCollisionHappened);
}

void DamageSystem::OnCollisionHappened(CollisionEvent& event)
{
    //Logger::Log("Collision With " + std::to_string(event.a.GetId()));
}
#endif