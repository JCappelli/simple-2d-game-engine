#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include<string>
#include <SDL2/SDL.h>

struct  SpriteComponent
{
public:
    int width;
    int height;
    SDL_Rect srcRect;
    std::string spriteAssetId;

    SpriteComponent(
        int width = 0, 
        int height = 0,
        int srcRectX = 0,
        int srcRectY = 0, 
        const std::string& assetId = "")
    {
        this->width = width;
        this->height = height;
        this->spriteAssetId = assetId;
        this->srcRect = {
            srcRectX,
            srcRectY,
            width,
            height
        };
    }
};

#endif