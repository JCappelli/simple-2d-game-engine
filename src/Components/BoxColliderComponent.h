#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

struct BoxColliderComponent
{
    int height;
    int width;
    int offsetX;
    int offsetY;
    bool isTrigger;

    BoxColliderComponent(int height = 10, int width = 10, int offsetX = 0, int offsetY = 0, bool isTrigger = false)
    {
        this->height = height;
        this->width = width;
        this->offsetX = offsetX;
        this->offsetY = offsetY;
        this->isTrigger = isTrigger;
    }
};


#endif