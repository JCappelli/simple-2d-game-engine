#ifndef ECS_H
#define ECS_H

#include<bitset>
#include<vector>
#include<unordered_map>
#include<typeindex>

const unsigned int MAX_COMPONENT_TYPES = 64;
typedef std::bitset<MAX_COMPONENT_TYPES> ComponentSignature;

struct BaseComponent
{
    protected:
        static int nextId;
};


template <typename T>
class Component : public BaseComponent
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
        std::vector<IPool*> componentPools;
        std::vector<ComponentSignature> entityComponentSignatures; //indexs are entity ID
        std::unordered_map<std::type_index, System> systems;
    
    public: 
        //TODO: need to be able to create an entity, destroy, add component, remove component
};

#endif