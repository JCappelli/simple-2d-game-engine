#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include<string>
#include <SDL2/SDL.h>

struct  SpriteComponent
{
public:
    int width;
    int height;
    int zIndex;
    SDL_Rect srcRect;
    std::string spriteAssetId;
    bool isXFlipped = false;

    SpriteComponent(
        int width = 0, 
        int height = 0,
        int srcRectX = 0,
        int srcRectY = 0,
        int zIndex = 0, 
        const std::string& assetId = "",
        bool isXFlipped = false)
    {
        this->width = width;
        this->height = height;
        this->spriteAssetId = assetId;
        this->zIndex = zIndex;
        this->srcRect = {
            srcRectX,
            srcRectY,
            width,
            height
        };
        this->isXFlipped = isXFlipped;
    }
};

#endif