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
        const glm::vec2& moveAxis, 
        std::set<Entity>& collisionsForObject);
    bool CheckCollisionAgainstAllOtherEntitiesInList(    
        const std::vector<Entity>& entities, 
        const Entity& a,
        const BoxColliderComponent& boxA, 
        const TransformComponent& transformA,
        Entity& entityOut,
        std::set<Entity>& collisionsForObject);

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
    std::set<Entity> collisonsForObject;
    for (auto i = entities.begin(); i != entities.end(); i++)
    {
        Entity a = *i;
        const auto boxA = a.GetComponent<BoxColliderComponent>();
        auto& transformA = a.GetComponent<TransformComponent>();
        
        if (a.HasComponent<RigidbodyComponent>())
        {
            auto& rigidbodyA = a.GetComponent<RigidbodyComponent>();
            collisonsForObject.clear();

            rigidbodyA.simulatedOffset += rigidbodyA.velocity * deltaTime;

            int moveX = std::floor(rigidbodyA.simulatedOffset.x);
            int moveY = std::floor(rigidbodyA.simulatedOffset.y);
            
            rigidbodyA.simulatedOffset -= glm::vec2(static_cast<float>(moveX), static_cast<float>(moveY));

            while (moveX != 0 || moveY != 0)
            {
                IterateMoveDirection(moveX, a, transformA, boxA, entities, glm::vec2(1,0), collisonsForObject);
                IterateMoveDirection(moveY, a, transformA, boxA, entities, glm::vec2(0,1), collisonsForObject);
            }

            for(auto collidedEntity : collisonsForObject)
            {
                eventBus->EmitEvent<CollisionEvent>(a, collidedEntity);
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
    const glm::vec2& moveAxis,
    std::set<Entity>& collisionsForObject)
{
    if (moveValue != 0)
    {
        bool collision = false;

        Entity otherEntity = movingEntity;
        int delta = glm::sign<int>(moveValue);
        TransformComponent tempTrans = movingTransform;
        tempTrans.position += moveAxis * static_cast<float>(delta);
        if  (CheckCollisionAgainstAllOtherEntitiesInList(entityList, movingEntity, movingBoxCollider, tempTrans, otherEntity, collisionsForObject))
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
    Entity& entityOut,
    std::set<Entity>& collisionsForObject)
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
            auto collisionIter = collisionsForObject.find(entityOut);
            if (collisionIter == collisionsForObject.end())
            {
                collisionsForObject.emplace_hint(collisionIter, entityOut);
            }
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
