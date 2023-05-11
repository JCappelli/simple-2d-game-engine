#ifndef ECS_H
#define ECS_H

#include<bitset>
#include<vector>
#include<unordered_map>
#include<typeindex>
#include<set>
#include<memory>

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
    public:
        Entity(int id): id(id) {};
        int GetId() const;

        bool operator== (const Entity& rhs) const
        {
            return GetId() == rhs.GetId();
        }
        bool operator< (const Entity& rhs) const
        {
            return GetId() < rhs.GetId();
        }
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

        std::vector<std::shared_ptr<IPool>> componentPools;
        std::vector<ComponentSignature> entityComponentSignatures; //indexs are entity ID
        std::unordered_map<std::type_index, std::shared_ptr<System>> systems;
    
    public: 
        //Entity
        Entity CreateEntity();

        //Components
        template <typename T, typename  ...TArgs> 
        void AddComponent(Entity entity, TArgs&& ...args)
        {
            const auto componentId = Component<T>::GetId();
            const auto entityId = entity.GetId();

            if (componentId >= componentPools.size())
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
        void HasComponent(Entity entity)
        {
            const auto componentId = Component<T>::GetId();
            const auto entityId = entity.GetId();

            return entityComponentSignatures[entityId].test(componentId);
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

        void AddEntityToSystems(Entity entity);
};

#endif