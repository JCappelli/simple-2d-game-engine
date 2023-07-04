#ifndef PROJECTILECOMPONENT_H
#define PROJECTILECOMPONENT_H

struct ProjectileComponent
{
    float maxRange;
    float currentTravelDistance;

    ProjectileComponent(float maxRange = 1)
    {
        this->maxRange = maxRange;
        this->currentTravelDistance = 0;
    }
};

#endif