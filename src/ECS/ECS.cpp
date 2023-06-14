#include "ECS.h"
#include "../Debugging/Logger.h"
#include <algorithm>

int IComponent::nextId = 0;

int Entity::GetId() const
{
    return id;
}

void Entity::Kill()
{
    registry->KillEntity(*this);
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

std::vector<Entity> System::GetSystemEntities() const
{
    return entities;
}

Entity Registry::CreateEntity()
{
    int entityId; //= numberOfEntities++;
    if (freeIds.empty())
    {
        entityId = numberOfEntities++;
        if (entityId >= static_cast<int>(entityComponentSignatures.size()))
        {
            entityComponentSignatures.resize(entityId + 1);
        }
    }
    else
    {
        entityId = freeIds.front();
        freeIds.pop_front();
    }

    Entity entity = Entity(entityId, this);
    entitiesToBeAdded.insert(entity);

    return entity;
}

void Registry::KillEntity(Entity entity)
{
    entitiesToBeRemoved.emplace(entity);
}

void Registry::AddEntityToSystems(Entity entity)
{
    const auto entityId = entity.GetId();
    const auto& entityComponentSignature = entityComponentSignatures[entityId];

    for (auto& system: systems)
    {
        const auto& systemComponentSignature = system.second->GetComponentSignature();

        bool isMatch = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

        if (isMatch)
        {
            system.second->AddEntityToSystem(entity);
        }
    }
}

void Registry::RemoveEntityFromSystems(Entity entity)
{
    for (auto systemPair: systems)
    {
        systemPair.second->RemoveEntityFromSystem(entity);
    }
}

void Registry::Update()
{
    //Handle Additions
    for (auto entity: entitiesToBeAdded)
    {
        AddEntityToSystems(entity);
    }
    entitiesToBeAdded.clear();

    //Handle Removals
    for(auto entity: entitiesToBeRemoved)
    {
        RemoveEntityFromSystems(entity);
        entityComponentSignatures[entity.GetId()].reset();
        
        freeIds.push_back(entity.GetId());
    }
    entitiesToBeRemoved.clear();
}
