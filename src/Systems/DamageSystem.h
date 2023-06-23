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
    DamageSystem(){
        RequireComponent<BoxColliderComponent>();
    }

    void SubscibeToEvents(std::unique_ptr<EventBus>& eventBus)
    {
        eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::OnCollisionHappened);
    }

    void OnCollisionHappened(CollisionEvent& event)
    {
        Logger::Log("Collision With " + std::to_string(event.a.GetId()));
    }
};

#endif