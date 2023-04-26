#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

class MovementSystem
{
private:
    /* data */
public:
    MovementSystem(); //Setup required components
    ~MovementSystem();

    void Update(); //Update Enity position based on velocity
};

MovementSystem::MovementSystem()
{
}

MovementSystem::~MovementSystem()
{
}

void MovementSystem::Update()
{

}

#endif