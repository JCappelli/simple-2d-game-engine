#ifndef PROJECTILESYSTEM_H
#define PROJECTILESYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/RigidbodyComponent.h"

class ProjectileSystem: public System
{
public:
    ProjectileSystem();

    void Update(float deltaTime);
};

ProjectileSystem::ProjectileSystem()
{
    RequireComponent<ProjectileComponent>();
    RequireComponent<RigidbodyComponent>();
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