#ifndef PROJECTILECOMPONENT_H
#define PROJECTILECOMPONENT_H

#include "../ECS/EntityTypes.h"

struct ProjectileComponent
{
    float maxRange;
    float currentTravelDistance;
    EntityFlags damageMask;
    int damageAmount;

    ProjectileComponent(float maxRange = 1, EntityFlags damageMask = EntityFlags::None, int damageAmount = 0)
    {
        this->maxRange = maxRange;
        this->currentTravelDistance = 0;
        this->damageMask = damageMask;
        this->damageAmount = damageAmount;
    }
};

#endif