#ifndef PROJECTILESYSTEM_H
#define PROJECTILESYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Events/CollisionEvent.h"
#include "../Components/HealthComponent.h"

class ProjectileSystem: public System
{
private:
    void OnCollisionHappened(CollisionEvent& collisionEvent);

public:
    ProjectileSystem();

    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus);
    void Update(float deltaTime);
};

ProjectileSystem::ProjectileSystem()
{
    RequireComponent<ProjectileComponent>();
    RequireComponent<RigidbodyComponent>();
}

void ProjectileSystem::SubscribeToEvents(std::unique_ptr<EventBus>& eventBus)
{
    eventBus->SubscribeToEvent<CollisionEvent>(this, &ProjectileSystem::OnCollisionHappened);
}

 void ProjectileSystem::OnCollisionHappened(CollisionEvent& collisionEvent)
 {
    Entity movingBody = collisionEvent.movingBody;
    Entity collidedBody = collisionEvent.collidedBody;

    if (movingBody.HasComponent<ProjectileComponent>() && 
        collidedBody.HasComponent<HealthComponent>())
    {
        const auto projectileComponent = movingBody.GetComponent<ProjectileComponent>();
        auto& healthComponent = collidedBody.GetComponent<HealthComponent>();

        if (collidedBody.HasFlags(projectileComponent.damageMask))
        {
            healthComponent.currentHealth -= projectileComponent.damageAmount;

            Logger::Log("Damaged Done");
            if (healthComponent.currentHealth <= 0)
            {
                collidedBody.Kill();

                Logger::Log("Entity Killed");
            }

            movingBody.Kill();
        }        
    }
 }

void ProjectileSystem::Update(float deltaTime)
{
    for (auto entity: GetSystemEntities())
    {
        auto& projectile = entity.GetComponent<ProjectileComponent>();
        const auto rigidbody = entity.GetComponent<RigidbodyComponent>();

        projectile.currentTravelDistance += glm::length(rigidbody.velocity) * deltaTime;
        if (projectile.currentTravelDistance > projectile.maxRange)
        {
            entity.Kill();
        }
    }
}
#endif