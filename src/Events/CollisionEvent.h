#ifndef COLLISIONEVENT_H
#define COLLISIONEVENT_H

#include "../ECS/ECS.h"
#include "Event.h"

struct CollisionEventData
{
    Entity movingBody;
    Entity collidedBody;

    CollisionEventData(Entity movingBody, Entity collidedBody): movingBody(movingBody), collidedBody(collidedBody) {} 
};

class CollisionEvent: public Event
{
public:
    CollisionEventData collisionData;
    
    CollisionEvent(Entity movingBody, Entity collidedBody): collisionData(CollisionEventData(movingBody, collidedBody)) {}

    CollisionEvent(CollisionEventData collisionEventData): collisionData(collisionEventData) {}
};

#endif