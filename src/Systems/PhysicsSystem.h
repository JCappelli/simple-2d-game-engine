#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/TransformComponent.h"
#include "../Events/EventBus.h"
#include "../Events/CollisionEvent.h"
#include <glm/glm.hpp>

class PhysicsSystem: public System
{
private:
    bool CheckAABBCollision(int aX, int aY, int aW, int aH, int bX, int bY, int bW, int bH) const;
    bool CheckCollisionAgainstAllOtherEntitiesInList(    
        const std::vector<Entity>& entities, 
        const Entity& a,
        const BoxColliderComponent& boxA, 
        const TransformComponent& transformA) const;
public:
    PhysicsSystem();

    void Update(float deltaTime, std::unique_ptr<EventBus>& eventBus);
};

PhysicsSystem::PhysicsSystem()
{
    RequireComponent<BoxColliderComponent>();
    RequireComponent<TransformComponent>();
}

void PhysicsSystem::Update(float deltaTime, std::unique_ptr<EventBus>& eventBus)
{
    std::vector<Entity> entities = GetSystemEntities();
    for (auto i = entities.begin(); i != entities.end(); i++)
    {
        Entity a = *i;
        const auto boxA = a.GetComponent<BoxColliderComponent>();
        auto& transformA = a.GetComponent<TransformComponent>();
        
        if (a.HasComponent<RigidbodyComponent>())
        {
            auto& rigidbodyA = a.GetComponent<RigidbodyComponent>();
            int moveX = static_cast<int>(transformA.position.x + (rigidbodyA.velocity.x * deltaTime)) -
                static_cast<int>(transformA.position.x);
            if (moveX == 0)
            {
                transformA.position.x += (rigidbodyA.velocity.x * deltaTime);
            }

            int moveY = static_cast<int>(transformA.position.y + (rigidbodyA.velocity.y * deltaTime)) -
                static_cast<int>(transformA.position.y);
            if (moveY == 0)
            {
                transformA.position.y += (rigidbodyA.velocity.y * deltaTime);
            }

            bool collision = false;
            while (moveX != 0 || moveY != 0)
            {
                if (moveX != 0)
                {
                    int delta = glm::sign<int>(moveX);
                    TransformComponent tempTrans = transformA;
                    tempTrans.position.x += static_cast<float>(delta);
                    if  (CheckCollisionAgainstAllOtherEntitiesInList(entities, a, boxA, tempTrans))
                    {
                        moveX = 0;
                        collision = true;
                    }
                    else
                    {
                        transformA = tempTrans;
                        moveX -= delta;
                    }
                }

                if (moveY != 0)
                {
                    int delta = glm::sign<int>(moveY);
                    TransformComponent tempTrans = transformA;
                    tempTrans.position.y += static_cast<float>(delta);
                    if  (CheckCollisionAgainstAllOtherEntitiesInList(entities, a, boxA, tempTrans))
                    {
                        moveY = 0;
                        collision = true;
                    }
                    else
                    {
                        transformA = tempTrans;
                        moveY -= delta;
                    }
                }
            }
            if (collision)
            {
                eventBus->EmitEvent<CollisionEvent>(a);
            }
        }
    }
}

bool PhysicsSystem::CheckCollisionAgainstAllOtherEntitiesInList(
    const std::vector<Entity>& entities, 
    const Entity& a,
    const BoxColliderComponent& boxA, 
    const TransformComponent& transformA) const
{
    for (auto other = entities.begin(); other != entities.end(); other++)
    {
        const Entity b = *other;
        if (a == b)
            continue;
        
        const auto boxB = b.GetComponent<BoxColliderComponent>();
        const auto transformB = b.GetComponent<TransformComponent>();

        //Check for collision between a and b
        bool colliding = CheckAABBCollision(
            static_cast<int>(transformA.position.x) + boxA.offsetX,
            static_cast<int>(transformA.position.y) + boxA.offsetY,
            boxA.width,
            boxA.height,
            static_cast<int>(transformB.position.x) + boxB.offsetX,
            static_cast<int>(transformB.position.y) + boxB.offsetY,
            boxB.width,
            boxB.height); 
        
        if (colliding)
            return true;
    }

    return false;
}

bool PhysicsSystem::CheckAABBCollision(int aX, int aY, int aW, int aH, int bX, int bY, int bW, int bH) const
{
    return(
        aX < bX + bW &&
        aX + aW > bX &&
        aY < bY + bH &&
        aY + aH > bY
    );
}

#endif
