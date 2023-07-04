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
    void IterateMoveDirection(
        int& moveValue, 
        const Entity& movingEntity, 
        TransformComponent& movingTransform, 
        const BoxColliderComponent& movingBoxCollider, 
        const std::vector<Entity>& entityList,
        const glm::vec2& moveAxis);
    bool CheckCollisionAgainstAllOtherEntitiesInList(    
        const std::vector<Entity>& entities, 
        const Entity& a,
        const BoxColliderComponent& boxA, 
        const TransformComponent& transformA,
        Entity& entityOut);

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

            rigidbodyA.simulatedOffset += rigidbodyA.velocity * deltaTime;

            int moveX = std::round(rigidbodyA.simulatedOffset.x);
            int moveY = std::round(rigidbodyA.simulatedOffset.y);
            
            rigidbodyA.simulatedOffset -= glm::vec2(static_cast<float>(moveX), static_cast<float>(moveY));

            while (moveX != 0 || moveY != 0)
            {
                IterateMoveDirection(moveX, a, transformA, boxA, entities, glm::vec2(1,0));
                IterateMoveDirection(moveY, a, transformA, boxA, entities, glm::vec2(0,1));
            }
        }
    }
}

void PhysicsSystem::IterateMoveDirection(
    int& moveValue, 
    const Entity& movingEntity, 
    TransformComponent& movingTransform, 
    const BoxColliderComponent& movingBoxCollider, 
    const std::vector<Entity>& entityList, 
    const glm::vec2& moveAxis)
{
    if (moveValue != 0)
    {
        bool collision = false;

        Entity otherEntity = movingEntity;
        int delta = glm::sign<int>(moveValue);
        TransformComponent tempTrans = movingTransform;
        tempTrans.position += moveAxis * static_cast<float>(delta);
        if  (CheckCollisionAgainstAllOtherEntitiesInList(entityList, movingEntity, movingBoxCollider, tempTrans, otherEntity))
        {
            collision = true;
        }

        BoxColliderComponent otherBox = otherEntity.GetComponent<BoxColliderComponent>();
        if (collision == false || movingBoxCollider.isTrigger || otherBox.isTrigger)
        {
            movingTransform = tempTrans;
            moveValue -= delta;
        }
        else
        {
            moveValue = 0;
        }
    }
}

bool PhysicsSystem::CheckCollisionAgainstAllOtherEntitiesInList(
    const std::vector<Entity>& entities, 
    const Entity& a,
    const BoxColliderComponent& boxA, 
    const TransformComponent& transformA, 
    Entity& entityOut)
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
        {
            entityOut = b;
            return true;
        }
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
