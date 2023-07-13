#ifndef ENTITYTYPES_H
#define ENTITYTYPES_H

#include <cstdint>
#include <type_traits>

enum class EntityFlags : std::uint8_t
{
    None = 0,
    Player = 1ull << 1,
    Enemy = 1ull << 2,
    Level = 1ull << 3
};

inline EntityFlags operator| (EntityFlags a, EntityFlags b)
{
    return static_cast<EntityFlags>(static_cast<std::underlying_type_t<EntityFlags>>(a) | 
        static_cast<std::underlying_type_t<EntityFlags>>(b));
}

inline EntityFlags operator& (EntityFlags a, EntityFlags b)
{
    return static_cast<EntityFlags>(static_cast<std::underlying_type_t<EntityFlags>>(a) & 
        static_cast<std::underlying_type_t<EntityFlags>>(b));
}

inline EntityFlags operator~ (EntityFlags a)
{
    return static_cast<EntityFlags>(~static_cast<std::underlying_type_t<EntityFlags>>(a));
}

#endif