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
        virtual ~IPool() {} // basically makes it an abstract class, need to define the destructor
};

template <typename T>
class ComponentPool : public IPool
{
    private:
        std::vector<T> components;
    public:
        ComponentPool(int size = 100){
            Resize(size);
        }

        virtual ~ComponentPool() = default;

        bool isEmpty() const { return components.empty(); }
        int GetSize() const  { return components.size(); }
        void Resize(int size) { components.resize(size); }
        void Clear() { components.clear(); }
        void Add(T newComponent) { components.push_back(newComponent); }
        void Set(int index, T componentValue) { components[index] = componentValue; }
        T& Get(int index) { return static_cast<T&>(components[index]); }
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

            if (entityId >= componentPool->GetSize())
            {
                componentPool->Resize(numberOfEntities);
            }

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