#include "Game.h"
#include "../Debugging/Logger.h"
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/InputSystem.h"
#include "../Debugging/DebugDrawCollidersSystem.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <fstream>
#include <string>
#include <ctype.h>

Game::Game()
{
    isRunning = false;
    Logger::Log("Game Created");
}

Game::~Game(){}

void Game::Initialize()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        Logger::Error("Failed to Init SDL");
        return;
    }

    window = SDL_CreateWindow(
        "Game", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        800, 
        600, 
        0);
    
    //For Fullscreen
    //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    if (!window)
    {
        Logger::Error("Error Creating SDL window");
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        Logger::Error("Error Creating SDL renderer");
        return;
    }

    isRunning = true;

    Setup();
}

void Game::Setup()
{
    //Create Asset Store
    assetStore = std::make_unique<AssetStore>();
    //Create Registry
    registry = std::make_unique<Registry>();
    //Create Event Bus
    eventBus = std::make_unique<EventBus>();

    //Setup Systems
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();
    registry->AddSystem<AnimationSystem>();
    registry->AddSystem<CollisionSystem>();
    registry->AddSystem<DamageSystem>();
    registry->AddSystem<InputSystem>();
    
    //Debug Systems
    registry->AddSystem<DebugDrawCollidersSystem>();

    LoadLevel();
}

void Game::LoadLevel()
{
    //Add Needed Textures
    assetStore->AddTexture(renderer, "tilemap",
        "./assets/sprites/mini-dungeon-tiles.png");

    //Load the tilemap
    int tileSize = 16;
    int tileMapWidth = 12;
    int mapSizeX = 30;
    int mapSizeY = 30;

    std::fstream mapFile;
    mapFile.open("./assets/tilemaps/level.csv");

    std::string workingNumberString = "";
    for (int y = 0; y < mapSizeY; y++)
    {
        for (int x = 0; x < mapSizeX; x++)
        {
            char c;
            mapFile.get(c);
            while (isdigit(c))
            {
                workingNumberString += c;
                c = '\0';
                mapFile.get(c);
            } 
            
            int tileIndex = std::atoi(workingNumberString.c_str());
            workingNumberString.clear();

            int srcRectY = (tileIndex / tileMapWidth) * tileSize;
            int srcRectX = (tileIndex - ((tileIndex / tileMapWidth) * tileMapWidth)) * tileSize;

            Entity tile = registry->CreateEntity();
            tile.AddComponent<TransformComponent>(glm::vec2(x * tileSize,y * tileSize), glm::vec2(1,1), 0.0);
            tile.AddComponent<SpriteComponent>(tileSize, tileSize, srcRectX, srcRectY, 1, "tilemap");
        }
    }
    mapFile.close();

    //Create Player
    Entity player = registry->CreateEntity();

    player.AddComponent<TransformComponent>( 
        glm::vec2(50,50), 
        glm::vec2(1,1),
        0.0);
    player.AddComponent<RigidbodyComponent>(
        glm::vec2(20,20));
    player.AddComponent<SpriteComponent>(
        16,
        16,
        1*16,
        8*16,
        2,
        "tilemap");
    player.AddComponent<AnimationComponent>(
        3,
        3,
        true);
    player.AddComponent<BoxColliderComponent>(
        16,
        16,
        0,
        0);


    Entity testCollisionEntity = registry->CreateEntity();
    testCollisionEntity.AddComponent<TransformComponent>(
        glm::vec2(16 * 4, 16 * 3),
        glm::vec2(1,1),
        0.0);
    testCollisionEntity.AddComponent<BoxColliderComponent>(
        64,
        80,
        0,
        0);

    Entity testKillTarget = registry->CreateEntity();
    testKillTarget.AddComponent<TransformComponent>();

    testKillTarget.Kill();
}

void Game::Run()
{
    Uint64 tickCountAfterLastUpdate = 0;
    while (isRunning)
    {
        while(!SDL_TICKS_PASSED(SDL_GetTicks64(), tickCountAfterLastUpdate + TARGET_MILLISECS_PER_FRAME));

        ProcessInput();
        Update(static_cast<float>(SDL_GetTicks64() - tickCountAfterLastUpdate) / 1000.0);
        Render();

        tickCountAfterLastUpdate = SDL_GetTicks64();
    }
}

void Game::Update(float deltaTime)
{
    eventBus->Reset();

    registry->GetSystem<DebugDrawCollidersSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<DamageSystem>().SubscibeToEvents(eventBus);

    //Update Systems
    registry->GetSystem<MovementSystem>().Update(deltaTime);
    registry->GetSystem<AnimationSystem>().Update();
    registry->GetSystem<CollisionSystem>().Update(eventBus);

    //Update entities (add/remove)
    registry->Update();
}

void Game::ProcessInput()
{
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent))
    {
        switch (sdlEvent.type)
        {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                registry->GetSystem<InputSystem>().PollKeyEvent(sdlEvent, eventBus);
                break;
            default:
                break;
        }
    }
}

void Game::Render()
{
    SDL_SetRenderDrawColor(renderer, 234, 165, 108, 255);
    SDL_RenderClear(renderer);

    registry->GetSystem<RenderSystem>().Update(renderer, assetStore);
    registry->GetSystem<DebugDrawCollidersSystem>().Update(renderer);

    SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
