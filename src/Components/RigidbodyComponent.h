#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include <glm/glm.hpp>

struct RigidbodyComponent
{
    glm::vec2 velocity;
    glm::vec2 simulatedOffset;

    RigidbodyComponent(glm::vec2 velocity = glm::vec2(0.0, 0.0))
    {
        this->velocity = velocity;
        this->simulatedOffset = glm::vec2(0,0);
    }
};

#endif