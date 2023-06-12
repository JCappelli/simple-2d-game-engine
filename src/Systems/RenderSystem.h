#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include <SDL2/SDL.h>
#include <algorithm>
#include "../AssetStore/AssetStore.h"

class RenderSystem : public System
{
public:
    const int SPRITE_RENDER_SCALE = 2;
    RenderSystem();
    ~RenderSystem();

    void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore);
};

RenderSystem::RenderSystem()
{
    RequireComponent<TransformComponent>();
    RequireComponent<SpriteComponent>();
}

RenderSystem::~RenderSystem()
{ 
}

void RenderSystem::Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore)
{
    SDL_SetRenderDrawColor(renderer, 234, 165, 108, 255);
    SDL_RenderClear(renderer);

    //Sort by z index
    std::vector<Entity> entities = GetSystemEntities();
    std::sort(entities.begin(), entities.end(), [](const Entity& a, const Entity& b)->bool
    {
        const auto spriteA = a.GetComponent<SpriteComponent>();
        const auto spriteB = b.GetComponent<SpriteComponent>();
        return (spriteA.zIndex < spriteB.zIndex);
    });

    //render all renderable entities
    for (auto entity: entities)
    {
        const auto transform = entity.GetComponent<TransformComponent>();
        const auto sprite = entity.GetComponent<SpriteComponent>();

        SDL_Rect srcRect = sprite.srcRect;

        SDL_Rect dstRect = {
            static_cast<int>(transform.position.x) * SPRITE_RENDER_SCALE,
            static_cast<int>(transform.position.y)* SPRITE_RENDER_SCALE,
            sprite.width * SPRITE_RENDER_SCALE * static_cast<int>(transform.scale.x),
            sprite.height * SPRITE_RENDER_SCALE *  static_cast<int>(transform.scale.y)
        };

        SDL_RenderCopyEx(renderer, 
            assetStore->GetTexture(sprite.spriteAssetId), 
            &srcRect, 
            &dstRect,
            static_cast<double>(transform.rotation), 
            NULL,
            SDL_RendererFlip::SDL_FLIP_NONE);
    }

    SDL_RenderPresent(renderer);
}

#endif