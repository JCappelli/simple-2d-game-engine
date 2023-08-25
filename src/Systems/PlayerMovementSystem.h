#ifndef PLAYERMOVEMENTSYSTEM_H
#define PLAYERMOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/PlayerMovementComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Events/EventBus.h"
#include "../Events/InputEvent.h"
#include <glm.hpp>

class PlayerMovementSystem: public System
{
private:
    glm::vec2 inputDirection;
    void OnButtonPressed(InputButtonEvent& event);

public:
    PlayerMovementSystem();
    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus);

    void Update();
};

PlayerMovementSystem::PlayerMovementSystem()
{
    inputDirection = glm::vec2(0,0);

    RequireComponent<RigidbodyComponent>();
    RequireComponent<PlayerMovementComponent>();
    RequireComponent<SpriteComponent>();
}

void PlayerMovementSystem::SubscribeToEvents(std::unique_ptr<EventBus>& eventBus)
{
    eventBus->SubscribeToEvent<InputButtonEvent>(this, &PlayerMovementSystem::OnButtonPressed);
}

void PlayerMovementSystem::Update()
{
    for (auto entity: GetSystemEntities())
    {
        auto& rigidBody = entity.GetComponent<RigidbodyComponent>();
        const auto playerMovement = entity.GetComponent<PlayerMovementComponent>();
        auto& spriteComponent = entity.GetComponent<SpriteComponent>();

        glm::vec2 clampedDirection = glm::vec2(0,0);
        float inputAmplitude = glm::clamp<float>(glm::length(inputDirection), 0.0, 1.0);
        if (inputAmplitude > 0)
        {
            clampedDirection = glm::normalize(inputDirection) * inputAmplitude; 
        }

        if (clampedDirection.x < 0)
        {
            spriteComponent.isXFlipped = true;
        }
        else if (clampedDirection.x > 0)
        {
            spriteComponent.isXFlipped = false;
        }

        rigidBody.velocity = clampedDirection * static_cast<float>(playerMovement.movementSpeed);
    }
}

void PlayerMovementSystem::OnButtonPressed(InputButtonEvent& event)
{
    if (event.actionType == INPUT_BUTTON_UP_PRESS || 
        event.actionType == INPUT_BUTTON_DOWN_RELEASE)
    {
        inputDirection += glm::vec2(0,-1); // Up and down reversed for SDL coordinates
    }
    else if (event.actionType == INPUT_BUTTON_DOWN_PRESS || 
            event.actionType == INPUT_BUTTON_UP_RELEASE)
    {
        inputDirection += glm::vec2(0,1);
    }
    else if (event.actionType == INPUT_BUTTON_LEFT_PRESS || 
            event.actionType == INPUT_BUTTON_RIGHT_RELEASE)
    {
        inputDirection += glm::vec2(-1, 0);
    }
    else if (event.actionType == INPUT_BUTTON_RIGHT_PRESS || 
            event.actionType == INPUT_BUTTON_LEFT_RELEASE)
    {
        inputDirection += glm::vec2(1, 0);
    }
}

#endif