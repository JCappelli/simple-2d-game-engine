#ifndef DEBUGDRAWCOLLIDERSSYSTEM_H
#define DEBUGDRAWCOLLIDERSSYSTEM_H

#include "../ECS/ECS.h"
#include "../Events/EventBus.h"
#include "../Events/InputEvent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Systems/RenderSystem.h"
#include <SDL2/SDL.h>

class DebugDrawCollidersSystem : public System
{
private:
    bool isEnabled = false;
    void OnButtonPressed(InputButtonEvent& event);

public:
    DebugDrawCollidersSystem();
    ~DebugDrawCollidersSystem() = default;

    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus);
    void Update(SDL_Renderer* renderer, const SDL_Rect& cameraRect);

};

DebugDrawCollidersSystem::DebugDrawCollidersSystem()
{
    RequireComponent<BoxColliderComponent>();
    RequireComponent<TransformComponent>();
}

void DebugDrawCollidersSystem::SubscribeToEvents(std::unique_ptr<EventBus>& eventBus)
{
    eventBus->SubscribeToEvent<InputButtonEvent>(this, &DebugDrawCollidersSystem::OnButtonPressed);
}

void DebugDrawCollidersSystem::OnButtonPressed(InputButtonEvent& event)
{
    if (event.actionType == INPUT_BUTTON_DEBUG_DRAW_PRESS)
    {
        isEnabled = !isEnabled;
    }
}

void DebugDrawCollidersSystem::Update(SDL_Renderer* renderer, const SDL_Rect& cameraRect)
{
    if (!isEnabled)
        return;

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    for (auto entity: GetSystemEntities())
    {
        const auto transform = entity.GetComponent<TransformComponent>();
        const auto box = entity.GetComponent<BoxColliderComponent>();

        SDL_Rect rect = {
            (static_cast<int>(transform.position.x) - cameraRect.x + box.offsetX) * RenderSystem::SPRITE_RENDER_SCALE,
            (static_cast<int>(transform.position.y) - cameraRect.y + box.offsetY) * RenderSystem::SPRITE_RENDER_SCALE,
            box.width * RenderSystem::SPRITE_RENDER_SCALE,
            box.height * RenderSystem::SPRITE_RENDER_SCALE,
        };

        SDL_RenderDrawRect(renderer, &rect);
    }
}
#endif