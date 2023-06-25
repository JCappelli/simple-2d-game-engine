#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Debugging/Logger.h"
#include "../Events/EventBus.h"
#include "../Events/CollisionEvent.h"

class CollisionSystem : public System
{
private:
    bool CheckAABBCollision(int aX, int aY, int aW, int aH, int bX, int bY, int bW, int bH);

public:
    CollisionSystem();
    ~CollisionSystem() = default;

    void Update(std::unique_ptr<EventBus>& eventBus);
};

CollisionSystem::CollisionSystem()
{
    RequireComponent<BoxColliderComponent>();
    RequireComponent<TransformComponent>();
}

void CollisionSystem::Update(std::unique_ptr<EventBus>& eventBus)
{
    std::vector<Entity> entities = GetSystemEntities();
    for (auto i = entities.begin(); i != entities.end(); i++)
    {
        Entity a = *i;
        const auto boxA = a.GetComponent<BoxColliderComponent>();
        const auto transformA = a.GetComponent<TransformComponent>();

        for (auto remaining = i; remaining != entities.end(); remaining++)
        {
            Entity b = *remaining;
            if (a == b)
            {
                continue;
            }

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
                boxB.height
            ); 

            if (colliding)
            {
                eventBus->EmitEvent<CollisionEvent>(a, b);
            }
        }
    }
}

bool CollisionSystem::CheckAABBCollision(int aX, int aY, int aW, int aH, int bX, int bY, int bW, int bH)
{
    return(
        aX < bX + bW &&
        aX + aW > bX &&
        aY < bY + bH &&
        aY + aH > bY
    );
}

#endif