#include "LevelLoader.h"

#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"

#include <fstream>
#include <sol/sol.hpp>

#include "../Debugging/Logger.h"
#include "GameObjectLoader.h"

void LevelLoader::LoadLevel(sol::state& lua, const std::string& levelFilePath, const std::unique_ptr<Registry>& registry, const std::unique_ptr<AssetStore>& assetStore, SDL_Renderer* renderer)
{
    assetStore->AddTexture(renderer, "tilemap",
        "./assets/sprites/mini-dungeon-tiles.png");

    sol::load_result levelScript = lua.load_file(levelFilePath);

    if (levelScript.valid() == false)
    {
        sol::error luaError = levelScript;
        std::string errorMessage = luaError.what();

        Logger::Log("Lua Error on Level Load: " + errorMessage);
        return;
    }

    sol::table levelTable = levelScript.call();

    std::string version = levelTable["version"];
    Logger::Log("Tiled Version " + version);

    int tileHeight = levelTable["tileheight"];
    int tileWidth = levelTable["tilewidth"];

    sol::table tilesets = levelTable["tilesets"];
    int tileMapStartOffset = tilesets[1]["firstgid"];

    int i = 1;
    sol::table layersTable = levelTable["layers"];
    while (true)
    {
        sol::optional<sol::table> hasLayer = layersTable[i];
        if (hasLayer == sol::nullopt)
            break;

        sol::table layer = layersTable[i];
        std::string layerType = layer["type"];

        if (layerType == "tilelayer")
        {
            CreateEntitiesFromTileLayer(layer, tileMapStartOffset, 12, tileHeight, tileWidth, registry);
        }
        else if (layerType == "objectgroup")
        {
            CreateEntitiesFromObjectLayer(layer, registry);
        }
        i++;
    }
}

void LevelLoader::CreateEntitiesFromTileLayer(sol::table& layer, const int tilesetStartOffset, const int tilesetWidth, const int tileHeight, const int tileWidth, const std::unique_ptr<Registry>& registry)
{
    int mapWidth = layer["width"];

    sol::table data = layer["data"];

    int i = 1;
    while (true)
    {
        sol::optional<int> hasMoreData = data[i];
        if (hasMoreData == sol::nullopt)
            break;

        int x = (i - 1) % mapWidth;
        int y = (i - 1) / mapWidth; 

        int tileIndex = data[i];
        tileIndex -= tilesetStartOffset;

        int srcRectY = (tileIndex / tilesetWidth) * tileHeight;
        int srcRectX = (tileIndex % tilesetWidth) * tileWidth;

        Entity tile = registry->CreateEntity();
        tile.AddComponent<TransformComponent>(glm::vec2(x * tileWidth, y * tileHeight), glm::vec2(1,1), 0.0);
        tile.AddComponent<SpriteComponent>(tileWidth, tileHeight, srcRectX, srcRectY, 1, "tilemap");
        
        i++;
    }
}

void LevelLoader::CreateEntitiesFromObjectLayer(sol::table& layer, const std::unique_ptr<Registry>& registry)
{
    sol::table objects = layer["objects"];
    std::string layerName = layer["name"];

    int i = 1;
    while (true)
    {
        sol::optional<sol::table> hasMoreData = objects[i];
        if (hasMoreData == sol::nullopt)
            break;

        sol::table object = objects[i];

        int x = object["x"];
        int y = object["y"];

        std::string name = object["name"];
        
        if (layerName == "ColliderLayer")
        {
            int width = object["width"];
            int height = object["height"];

            GameObjectLoader::LoadCollisionEntity(x, y, width, height, registry);
        }
        else
        {
            if (name == "PlayerSpawn")
            {
                GameObjectLoader::LoadPlayerEntity(x, y, registry);
            }
            else if (name == "EnemySpawn")
            {
                GameObjectLoader::LoadEnemyEntity(x, y, registry);
            }
        }
        
        i++;
    }
}