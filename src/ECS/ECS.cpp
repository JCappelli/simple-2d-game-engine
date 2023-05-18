#include "ECS.h"
#include "../Debugging/Logger.h"
#include <algorithm>

int IComponent::nextId = 0;

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

Entity Registry::CreateEntity()
{
    int entityId = numberOfEntities++;

    Entity entity = Entity(entityId, this);
    entitiesToBeAdded.insert(entity);

    if (entityId >= static_cast<int>(entityComponentSignatures.size()))
    {
        entityComponentSignatures.resize(entityId + 1);
    }

    Logger::Log("Created entity with id: " + std::to_string(entityId));

    return entity;
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

void Registry::Update()
{
    for (auto entity: entitiesToBeAdded)
    {
        AddEntityToSystems(entity);
    }
    entitiesToBeAdded.clear();

    //Remove Entities based on queue
}
