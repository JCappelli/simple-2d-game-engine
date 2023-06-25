#ifndef PLAYERMOVEMENTCOMPONENT_H
#define PLAYERMOVEMENTCOMPONENT_H

struct PlayerMovementComponent
{
    int movementSpeed;
    
    PlayerMovementComponent(int movementSpeed = 0)
    {
        this->movementSpeed = movementSpeed;
    }
};
#endif