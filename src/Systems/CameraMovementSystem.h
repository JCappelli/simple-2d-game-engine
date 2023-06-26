#ifndef CAMERAMOVEMENTSYSTEM_H
#define CAMERAMOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/TransformComponent.h"
#include <SDL2/SDL.h>

class CameraMovementSystem: public System
{
public:
    CameraMovementSystem();

    void Update(SDL_Rect& cameraRect);
};

CameraMovementSystem::CameraMovementSystem()
{
    RequireComponent<CameraFollowComponent>();
    RequireComponent<TransformComponent>();
}

void  CameraMovementSystem::Update(SDL_Rect& cameraRect)
{
    for (auto entity: GetSystemEntities())
    {
        const auto transform = entity.GetComponent<TransformComponent>();

        cameraRect.x = transform.position.x - (cameraRect.w * 0.5) / RenderSystem::SPRITE_RENDER_SCALE;
        cameraRect.y = transform.position.y - (cameraRect.h * 0.5) / RenderSystem::SPRITE_RENDER_SCALE;
    }
}

#endif