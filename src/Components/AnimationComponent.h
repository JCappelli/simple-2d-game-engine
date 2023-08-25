#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include <SDL.h>

struct AnimationComponent
{
    int numberOfFrames;
    int currentFrame;
    int frameRate;
    bool isLooping;
    int startTime;

    AnimationComponent(int numFrames = 1, int frameRate = 1, bool isLooping = false)
    {
        this->numberOfFrames = numFrames;
        this->currentFrame = 0;
        this->frameRate = frameRate;
        this->isLooping = isLooping;
        this->startTime = SDL_GetTicks();
    }
};

#endif