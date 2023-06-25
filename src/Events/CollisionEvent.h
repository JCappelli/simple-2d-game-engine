#ifndef COLLISIONEVENT_H
#define COLLISIONEVENT_H

#include "../ECS/ECS.h"
#include "Event.h"

class CollisionEvent: public Event
{
public:
    Entity collisionSubject;
    CollisionEvent(Entity collisionSubject): collisionSubject(collisionSubject) {}
};

#endif