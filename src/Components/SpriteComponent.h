#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include<string>

struct  SpriteComponent
{
public:
    int width;
    int height;
    std::string spriteAssetId;

    SpriteComponent(
        int width = 0, 
        int height = 0, 
        const std::string& assetId = "")
    {
        this->width = width;
        this->height = height;
        this->spriteAssetId = assetId;
    }
};

#endif