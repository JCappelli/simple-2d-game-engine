#ifndef DEBUGDRAWCOLLIDERSSYSTEM_H
#define DEBUGDRAWCOLLIDERSSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Systems/RenderSystem.h"
#include <SDL2/SDL.h>

class DebugDrawCollidersSystem : public System
{
public:
    DebugDrawCollidersSystem();
    ~DebugDrawCollidersSystem() = default;

    void Update(SDL_Renderer* renderer);
};

DebugDrawCollidersSystem::DebugDrawCollidersSystem()
{
    RequireComponent<BoxColliderComponent>();
    RequireComponent<TransformComponent>();
}

void DebugDrawCollidersSystem::Update(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    for (auto entity: GetSystemEntities())
    {
        const auto transform = entity.GetComponent<TransformComponent>();
        const auto box = entity.GetComponent<BoxColliderComponent>();

        SDL_Rect rect = {
            (static_cast<int>(transform.position.x) + box.offsetX) * RenderSystem::SPRITE_RENDER_SCALE,
            (static_cast<int>(transform.position.y) + box.offsetY) * RenderSystem::SPRITE_RENDER_SCALE,
            box.width * RenderSystem::SPRITE_RENDER_SCALE,
            box.height * RenderSystem::SPRITE_RENDER_SCALE,
        };

        SDL_RenderDrawRect(renderer, &rect);
    }
}

#endif