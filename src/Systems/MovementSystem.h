#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Debugging/Logger.h"

class MovementSystem : public System
{
private:
    /* data */
public:
    MovementSystem();
    ~MovementSystem();

    void Update(float deltaTime); //Update Enity position based on velocity
};

MovementSystem::MovementSystem()
{
    RequireComponent<TransformComponent>();
    RequireComponent<RigidbodyComponent>();
}

MovementSystem::~MovementSystem()
{
}

void MovementSystem::Update(float deltaTime)
{
    for(auto entity: GetSystemEntities())
    {
        TransformComponent& transform = entity.GetComponent<TransformComponent>();
        const RigidbodyComponent rigidbody = entity.GetComponent<RigidbodyComponent>();

        transform.position += rigidbody.velocity * deltaTime;
    }
}

#endif