#ifndef PLAYERSHOOTINGCOMPONENT_H
#define PLAYERSHOOTINGCOMPONENT_H

#include <string>

struct PlayerShootingComponent
{
    int spriteSrcRectX;
    int spriteSrcRectY;
    std::string spriteAssetId;
    float maxRange;
    float projectileSpeed;

    PlayerShootingComponent(
        int spriteSrcRectX = 0,
        int spriteSrcRectY = 0,
        std::string spriteAssetId = "",
        float maxRange = 0, 
        float projectileSpeed = 0)
    {
        this->spriteSrcRectX = spriteSrcRectX;
        this->spriteSrcRectY = spriteSrcRectY;
        this->spriteAssetId = spriteAssetId;
        this->maxRange = maxRange;
        this->projectileSpeed = projectileSpeed;
    }
};

#endif