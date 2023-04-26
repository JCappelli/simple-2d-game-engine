#include "ECS.h"
#include <algorithm>

int Entity::GetId() const
{
    return id;
}

void System::AddEntityToSystem(Entity entity)
{
    entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity)
{
    std::remove(entities.begin(), entities.end(), entity);
}

const ComponentSignature& System::GetComponentSignature() const
{
    return requiredSignature;
}