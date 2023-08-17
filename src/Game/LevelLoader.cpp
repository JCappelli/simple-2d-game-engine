#include "LevelLoader.h"

#include "../Components/TransformComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/PlayerMovementComponent.h"
#include "../Components/PlayerShootingComponent.h"
#include "../Components/HealthComponent.h"

#include <fstream>
#include <sol/sol.hpp>

#include "../Debugging/Logger.h"

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
        else if (layerType == "objectGroup")
        {
            //CreateEntitiesFromObjectLayer(layer, registry);
        }
        i++;
    }


    // //Load the tilemap
    // int tileSize = 16;
    // int tileMapWidth = 12;
    // int mapSizeX = 30;
    // int mapSizeY = 30;

    // std::fstream mapFile;
    // mapFile.open("./assets/tilemaps/level.csv");

    // std::string workingNumberString = "";
    // for (int y = 0; y < mapSizeY; y++)
    // {
    //     for (int x = 0; x < mapSizeX; x++)
    //     {
    //         char c;
    //         mapFile.get(c);
    //         while (isdigit(c))
    //         {
    //             workingNumberString += c;
    //             c = '\0';
    //             mapFile.get(c);
    //         } 
            
    //         int tileIndex = std::atoi(workingNumberString.c_str());
    //         workingNumberString.clear();

    //         int srcRectY = (tileIndex / tileMapWidth) * tileSize;
    //         int srcRectX = (tileIndex - ((tileIndex / tileMapWidth) * tileMapWidth)) * tileSize;

    //         Entity tile = registry->CreateEntity();
    //         tile.AddComponent<TransformComponent>(glm::vec2(x * tileSize,y * tileSize), glm::vec2(1,1), 0.0);
    //         tile.AddComponent<SpriteComponent>(tileSize, tileSize, srcRectX, srcRectY, 1, "tilemap");
    //     }
    // }
    // mapFile.close();

    //Create Player
    // Entity player = registry->CreateEntity();
    // player.AddFlags(EntityFlags::Player);
    // player.AddComponent<TransformComponent>( 
    //     glm::vec2(2 * 16, 2 * 16), 
    //     glm::vec2(1,1),
    //     0.0);
    // player.AddComponent<RigidbodyComponent>(
    //     glm::vec2(20,20));
    // player.AddComponent<PlayerMovementComponent>(
    //     75);
    // player.AddComponent<PlayerShootingComponent>(
    //     5*16,
    //     8*16,
    //     "tilemap",
    //     160,
    //     200, 
    //     5);
    // player.AddComponent<SpriteComponent>(
    //     16,
    //     16,
    //     0*16,
    //     7*16,
    //     2,
    //     "tilemap");
    // player.AddComponent<AnimationComponent>(
    //     2,
    //     5,
    //     true);
    // player.AddComponent<BoxColliderComponent>(
    //     16,
    //     16,
    //     0,
    //     0);
    // player.AddComponent<CameraFollowComponent>();

    // Entity testCollisionEntity = registry->CreateEntity();
    // testCollisionEntity.AddComponent<TransformComponent>(
    //     glm::vec2(16 * 4, 16 * 3),
    //     glm::vec2(1,1),
    //     0.0);
    // testCollisionEntity.AddComponent<BoxColliderComponent>(
    //     64,
    //     80,
    //     0,
    //     0);
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

        int x = i % mapWidth;
        int y = i / mapWidth; 

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

}