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
    glm::vec2 aimTargetScreenPos;

    void OnButtonPush(InputButtonEvent& buttonEvent);
    void OnCursorMoved(InputCursorEvent& cursorEvent);

public:
    PlayerShootingSystem();
    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus);
    void Update(std::unique_ptr<Registry>& registry, const SDL_Rect& cameraRect);
};

PlayerShootingSystem::PlayerShootingSystem()
{
    shouldShoot = false;
    aimTargetScreenPos = glm::vec2(0,0);

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
    if (buttonEvent.actionType == INPUT_BUTTON_SHOOT_PRESS)
    {
        shouldShoot = true;
    }
}

void PlayerShootingSystem::OnCursorMoved(InputCursorEvent& cursorEvent)
{
    aimTargetScreenPos = glm::vec2(static_cast<float>(cursorEvent.xPosition), 
        static_cast<float>(cursorEvent.yPosition));
}

void PlayerShootingSystem::Update(std::unique_ptr<Registry>& registry, const SDL_Rect& cameraRect)
{
    if (shouldShoot)
    {
        std::vector<Entity> entities = GetSystemEntities();
        if (entities.size() > 0)
        {
            Entity player = entities[0];
            const auto transform = player.GetComponent<TransformComponent>();

            glm::vec2 aimTargetWorldPos = aimTargetScreenPos /
                static_cast<float>(RenderSystem::SPRITE_RENDER_SCALE) + glm::vec2(cameraRect.x, cameraRect.y);

            glm::vec2 bulletVelocity = glm::normalize(aimTargetWorldPos - transform.position);
            bulletVelocity *= 100;
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
            bullet.AddComponent<BoxColliderComponent>(
                10,
                10,
                6,
                6,
                true);
        }

        shouldShoot = false;
    }
}
#endif