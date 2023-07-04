#ifndef COLLISIONEVENT_H
#define COLLISIONEVENT_H

#include "../ECS/ECS.h"
#include "Event.h"

class CollisionEvent: public Event
{
public:
    Entity movingBody;
    Entity collidedBody;
    CollisionEvent(Entity movingBody, Entity collidedBody): movingBody(movingBody), collidedBody(collidedBody) {}
};

#endif