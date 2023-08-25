#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include <SDL.h>
#include <algorithm>
#include "../AssetStore/AssetStore.h"

class RenderSystem : public System
{
private:
    struct ZCompare
    {
        bool operator()(const Entity& a, const Entity& b) const
        {
            const auto spriteA = a.GetComponent<SpriteComponent>();
            const auto spriteB = b.GetComponent<SpriteComponent>();
            return (spriteA.zIndex < spriteB.zIndex);
        }
    };

public:
    static const int SPRITE_RENDER_SCALE = 2;
    RenderSystem();
    ~RenderSystem();

    void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, const SDL_Rect& cameraRect);

    std::multiset<Entity, RenderSystem::ZCompare> renderableEntities;
};

RenderSystem::RenderSystem()
{
    RequireComponent<TransformComponent>();
    RequireComponent<SpriteComponent>();
}

RenderSystem::~RenderSystem()
{ 
}

void RenderSystem::Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, const SDL_Rect& cameraRect)
{
    //Sort by z index and add only what's in the camera rect
    std::vector<Entity> entities = GetSystemEntities();
    renderableEntities.clear();
    for(auto entity: entities)
    {
        const auto transform = entity.GetComponent<TransformComponent>();

        if (transform.position.x > cameraRect.x || 
            transform.position.x < cameraRect.x + cameraRect.w ||
            transform.position.y > cameraRect.y ||
            transform.position.y < cameraRect.y + cameraRect.h)
        {
            renderableEntities.emplace(entity);
        }
    }

    //render all renderable entities
    for (auto entity: renderableEntities)
    {
        const auto transform = entity.GetComponent<TransformComponent>();
        const auto sprite = entity.GetComponent<SpriteComponent>();

        SDL_Rect srcRect = sprite.srcRect;

        SDL_Rect dstRect = {
            static_cast<int>(transform.position.x - cameraRect.x) * SPRITE_RENDER_SCALE,
            static_cast<int>(transform.position.y - cameraRect.y)* SPRITE_RENDER_SCALE,
            sprite.width * SPRITE_RENDER_SCALE * static_cast<int>(transform.scale.x),
            sprite.height * SPRITE_RENDER_SCALE *  static_cast<int>(transform.scale.y)
        };

        SDL_RenderCopyEx(renderer, 
            assetStore->GetTexture(sprite.spriteAssetId), 
            &srcRect, 
            &dstRect,
            static_cast<double>(transform.rotation), 
            NULL,
            sprite.isXFlipped ? SDL_RendererFlip::SDL_FLIP_HORIZONTAL : SDL_RendererFlip::SDL_FLIP_NONE);
    }
}

#endif