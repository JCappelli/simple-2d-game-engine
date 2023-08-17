#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <sol/sol.hpp>

class LevelLoader
{
private:
    static void CreateEntitiesFromTileLayer(sol::table& layer, const int tilesetStartOffset, const int tilesetWidth, const int tileHeight, const int tileWidth, const std::unique_ptr<Registry>& registry);
    static void CreateEntitiesFromObjectLayer(sol::table& layer, const std::unique_ptr<Registry>& registry);
public:
    static void LoadLevel(sol::state& lua, const std::string& levelFilePath, const std::unique_ptr<Registry>& registry, const std::unique_ptr<AssetStore>& assetStore, SDL_Renderer* renderer);
};

#endif