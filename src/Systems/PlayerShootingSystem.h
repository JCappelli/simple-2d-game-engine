#ifndef PLAYERSHOOTINGSYSTEM_H
#define PLAYERSHOOTINGSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/PlayerMovementComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Events/EventBus.h"
#include "../Events/InputEvent.h"
#include <glm/glm.hpp>

class PlayerShootingSystem: public System
{
private:    
    bool shouldShoot = false;
    glm::vec2 aimTarget;

    void OnButtonPush(InputButtonEvent& buttonEvent);
    void OnCursorMoved(InputCursorEvent& cursorEvent);

public:
    PlayerShootingSystem();
    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus);
    void Update(std::unique_ptr<Registry>& registry);
};

PlayerShootingSystem::PlayerShootingSystem()
{
    shouldShoot = false;
    aimTarget = glm::vec2(0,0);

    RequireComponent<PlayerMovementComponent>();
    RequireComponent<TransformComponent>();
}

void PlayerShootingSystem::SubscribeToEvents(std::unique_ptr<EventBus>& eventBus)
{
    eventBus->SubscribeToEvent<InputButtonEvent>(this, &PlayerShootingSystem::OnButtonPush);
    eventBus->SubscribeToEvent<InputCursorEvent>(this, &PlayerShootingSystem::OnCursorMoved);
}

void PlayerShootingSystem::OnButtonPush(InputButtonEvent& buttonEvent)
{
    shouldShoot = true;
}

void PlayerShootingSystem::OnCursorMoved(InputCursorEvent& cursorEvent)
{
    aimTarget = glm::vec2(cursorEvent.xPosition, cursorEvent.yPosition);
}

void PlayerShootingSystem::Update(std::unique_ptr<Registry>& registry, SDL_Rect cameraRect)
{
    if (shouldShoot)
    {
        std::vector<Entity> entities = GetSystemEntities();
        if (entities.size() > 0)
        {
            Entity player = entities[0];
            const auto transform = player.GetComponent<TransformComponent>();
            
            glm::vec2 bulletVelocity = glm::normalize(aimTarget - transform.position);
            bulletVelocity *= 10;
            Entity bullet = registry->CreateEntity();
            bullet.AddComponent<TransformComponent>(
                transform.position, 
                glm::vec2(1,1),
                0.0);
            bullet.AddComponent<SpriteComponent>(
                16,
                16,
                5*16,
                8*16,
                3,
                "tilemap");
            bullet.AddComponent<RigidbodyComponent>(
                bulletVelocity);
        }

        shouldShoot = false;
    }
}
#endif