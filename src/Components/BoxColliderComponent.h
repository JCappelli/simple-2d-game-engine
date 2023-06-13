#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

struct BoxColliderComponent
{
    int height;
    int width;
    int offsetX;
    int offsetY;

    BoxColliderComponent(int height = 10, int width = 10, int offsetX = 0, int offsetY = 0)
    {
        this->height = height;
        this->width = width;
        this->offsetX = offsetX;
        this->offsetY = offsetY;
    }
};


#endif