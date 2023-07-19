#ifndef ECS_H
#define ECS_H

#include<bitset>
#include<vector>
#include<unordered_map>
#include<typeindex>
#include<set>
#include<memory>
#include<deque>
#include "../Debugging/Logger.h"
#include "EntityTypes.h"

const unsigned int MAX_COMPONENT_TYPES = 64;
typedef std::bitset<MAX_COMPONENT_TYPES> ComponentSignature;

struct IComponent
{
    protected:
        static int nextId;
};

template <typename T>
class Component : public IComponent
{
    public:
        static int GetId()
        {
            static int id = nextId++;
            return id;
        }
};

class Entity
{
    private:
        int id;
        class Registry* registry;

    public:
        Entity(int id, Registry* registry): id(id), registry(registry) {};
        int GetId() const;
        void Kill();

        bool operator== (const Entity& rhs) const
        {
            return GetId() == rhs.GetId();
        }
        bool operator< (const Entity& rhs) const
        {
            return GetId() < rhs.GetId();
        }


        void AddFlags(EntityFlags flags);
        void RemoveFlags(EntityFlags flags);
        void ClearFlags();
        bool HasFlags(EntityFlags flags);

        template<typename T, typename ...TArgs> void AddComponent(TArgs&& ...args);
        template<typename T> void RemoveComponent();
        template<typename T> bool HasComponent() const;
        template<typename T> T& GetComponent() const;
};

class System
{
    private: 
        ComponentSignature requiredSignature;
        std::vector<Entity> entities;
    
    public:
        void AddEntityToSystem(Entity entity);
        void RemoveEntityFromSystem(Entity entity);
        const ComponentSignature& GetComponentSignature() const;
        std::vector<Entity> GetSystemEntities() const;

        //Defines Component type that entities need to have 
        //to be used by the system
        template <typename T> void RequireComponent()
        {
            const int componentId = Component<T>::GetId();
            requiredSignature.set(componentId);
        }
};

class IPool {
    public:
        virtual ~IPool() = default; // basically makes it an abstract class, need to define the destructor
        virtual void RemoveEntityFromPool(int entityId) = 0;
};

template <typename T>
class ComponentPool : public IPool
{
    private:
        std::vector<T> components;
        int size;

        std::unordered_map<int, int> entityToIndex;
        std::unordered_map<int, int> indexToEntity;

    public:
        ComponentPool(int capacity = 100){
            size = 0;
            components.resize(capacity);
        }

        virtual ~ComponentPool() = default;

        bool IsEmpty() const 
        {
            return size == 0;
        }

        int GetSize() const  
        { 
            return size; 
        }

        void Clear() 
        { 
            components.clear();
            entityToIndex.clear();
            indexToEntity.clear();
            size = 0;
        }

        void Set(int entityId, T componentValue) 
        { 
            if (entityToIndex.find(entityId) != entityToIndex.end())
            {
                int index = entityToIndex[entityId];
                components[index] = componentValue;
            }
            else
            {
                int index = size;
                entityToIndex.emplace(entityId, index);
                indexToEntity.emplace(index, entityId);
                if (index >= static_cast<int>(components.capacity()))
                {
                    components.resize(size * 2);
                }
                components[index] = componentValue;
                size++;
            }
        }

        void Remove(int entityId)
        {
            int indexOfRemoved = entityToIndex[entityId];
            int indexOfLast = size - 1;
            components[indexOfRemoved] = components[indexOfLast];

            int entityIdOfLast = indexToEntity[indexOfLast];
            entityToIndex[entityIdOfLast] = indexOfRemoved;
            indexToEntity[indexOfRemoved] = entityIdOfLast;

            entityToIndex.erase(entityId);
            indexToEntity.erase(indexOfLast);

            size--;
        }

        T& Get(int entityId) 
        { 
            int index = entityToIndex[entityId];
            return static_cast<T&>(components[index]); 
        }

        void RemoveEntityFromPool(int entityId) override
        {
            if (entityToIndex.find(entityId) != entityToIndex.end())
            {
                Remove(entityId);
            }
        }
};

class Registry
{
    private:
        int numberOfEntities = 0;
        std::set<Entity> entitiesToBeAdded;
        std::set<Entity> entitiesToBeRemoved;
        std::deque<int> freeIds;

        std::vector<std::shared_ptr<IPool>> componentPools;
        std::vector<ComponentSignature> entityComponentSignatures; //indexs are entity ID
        std::unordered_map<std::type_index, std::shared_ptr<System>> systems;
    
        std::unordered_map<int, EntityFlags> entityFlagsMap;

        void AddEntityToSystems(Entity entity);
        void RemoveEntityFromSystems(Entity entity);

    public: 
        //Entity
        Entity CreateEntity();
        void KillEntity(Entity entity);

        void AddEntityFlags(Entity entity, EntityFlags flags);
        void RemoveFlags(Entity entity, EntityFlags flags);
        void ClearFlags(Entity entity);
        bool HasFlags(Entity entity, EntityFlags flags);

        //Components
        template <typename T, typename  ...TArgs> 
        void AddComponent(Entity entity, TArgs&& ...args)
        {
            const auto componentId = Component<T>::GetId();
            const auto entityId = entity.GetId();

            if (componentId >= static_cast<int>(componentPools.size()))
            {
                componentPools.resize(componentId + 1, nullptr);
            }

            if (componentPools[componentId] == nullptr)
            {
                std::shared_ptr<ComponentPool<T>> newComponentPool = std::make_shared<ComponentPool<T>>();
                componentPools[componentId] = newComponentPool;
            }

            std::shared_ptr<ComponentPool<T>> componentPool = 
                std::static_pointer_cast<ComponentPool<T>>(componentPools[componentId]);

            T newComponent = T(std::forward<TArgs>(args)...);

            componentPool->Set(entityId, newComponent);

            entityComponentSignatures[entityId].set(componentId);
        }

        template <typename T>
        void RemoveComponent(Entity entity)
        {
            const auto componentId = Component<T>::GetId();
            const auto entityId = entity.GetId();

            entityComponentSignatures[entityId].set(componentId, false);
            
            std::shared_ptr<ComponentPool<T>> componentPool = 
                std::static_pointer_cast<ComponentPool<T>>(componentPools[componentId]);
            componentPool->Remove(entityId);
        }

        template <typename T>
        bool HasComponent(Entity entity)
        {
            const auto componentId = Component<T>::GetId();
            const auto entityId = entity.GetId();

            return entityComponentSignatures[entityId].test(componentId);
        }

        template <typename T>
        T& GetComponent(Entity entity)
        {
            const auto componentId = Component<T>::GetId();
            const auto entityId = entity.GetId();

            auto componentPool = std::static_pointer_cast<ComponentPool<T>>(componentPools[componentId]);
            return componentPool->Get(entityId);
        }

        void Update();

        //Systems
        template <typename T, typename ...TArgs>
        void AddSystem(TArgs&& ...args)
        {
            std::shared_ptr<T> newSystem = std::make_shared<T>(std::forward<TArgs>(args)...);

            systems.insert(std::make_pair(std::type_index(typeid(T)),newSystem));
        }

        template <typename T>
        void RemoveSystem()
        {
            auto systemIterator = systems.find(std::type_index(typeid(T)));
            systems.erase(systemIterator);
        }

        template <typename T>
        bool HasSystem() const
        {
            return systems.find(std::type_index(typeid(T))) != systems.end();
        }

        template <typename T>
        T& GetSystem() const
        {
            auto systemIterator = systems.find(std::type_index(typeid(T)));
            return *(std::static_pointer_cast<T>(systemIterator->second));
        }
};

template<typename T, typename ...TArgs> 
void Entity::AddComponent(TArgs&& ...args)
{
    registry->AddComponent<T>(*this, std::forward<TArgs>(args)...);
}

template<typename T>
void Entity::RemoveComponent()
{
    registry->RemoveComponent<T>(*this);
}

template<typename T>
bool Entity::HasComponent() const
{
    return registry->HasComponent<T>(*this);
}

template<typename T>
T& Entity::GetComponent() const
{
    return registry->GetComponent<T>(*this);
}

#endif