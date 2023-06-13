#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"

class AnimationSystem : public System
{
public:
    AnimationSystem();
    ~AnimationSystem() = default;

    void Update();
};

AnimationSystem::AnimationSystem()
{
    RequireComponent<SpriteComponent>();
    RequireComponent<AnimationComponent>();
}

void AnimationSystem::Update()
{
    int time = SDL_GetTicks();
    for (auto entity: GetSystemEntities())
    {
        auto& spriteComponent = entity.GetComponent<SpriteComponent>();
        auto& animationComponent = entity.GetComponent<AnimationComponent>();

        animationComponent.currentFrame = (((time - animationComponent.startTime) * animationComponent.frameRate) / 1000)
             % animationComponent.numberOfFrames;

        spriteComponent.srcRect.x = animationComponent.currentFrame * spriteComponent.width;
    }
}

#endif