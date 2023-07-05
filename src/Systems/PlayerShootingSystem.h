#ifndef PLAYERSHOOTINGSYSTEM_H
#define PLAYERSHOOTINGSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/PlayerShootingComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/ProjectileComponent.h"
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

    RequireComponent<PlayerShootingComponent>();
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
        for (auto entity: GetSystemEntities())
        {
            const auto transform = entity.GetComponent<TransformComponent>();
            const auto playerShooting = entity.GetComponent<PlayerShootingComponent>();

            glm::vec2 aimTargetWorldPos = aimTargetScreenPos /
                static_cast<float>(RenderSystem::SPRITE_RENDER_SCALE) + glm::vec2(cameraRect.x, cameraRect.y);

            glm::vec2 bulletVelocity = glm::normalize(aimTargetWorldPos - transform.position);
            bulletVelocity *= playerShooting.projectileSpeed;
            Entity bullet = registry->CreateEntity();
            bullet.AddComponent<TransformComponent>(
                transform.position, 
                glm::vec2(1,1),
                0.0);
            bullet.AddComponent<SpriteComponent>(
                16,
                16,
                playerShooting.spriteSrcRectX,
                playerShooting.spriteSrcRectY,
                3,
                playerShooting.spriteAssetId);
            bullet.AddComponent<RigidbodyComponent>(
                bulletVelocity);
            bullet.AddComponent<ProjectileComponent>(
                playerShooting.maxRange);
            bullet.AddComponent<BoxColliderComponent>(
                10,
                10,
                3,
                3,
                true);
        }

        shouldShoot = false;
    }
}
#endif