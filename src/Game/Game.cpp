#include "Game.h"
#include "../Debugging/Logger.h"
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/MovementSystem.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>

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

    //Add Needed Textures
    assetStore->AddTexture(renderer, "tilemap",
        "./assets/sprites/mini-dungeon-tiles.png");

    //Create Registry
    registry = std::make_unique<Registry>();

    //Setup Systems
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();

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
        "tilemap");
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
    //Update Systems
    registry->GetSystem<MovementSystem>().Update(deltaTime);

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
                if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
                {
                    isRunning = false;
                }
                break;
            default:
                break;
        }
    }
}

void Game::Render()
{
    registry->GetSystem<RenderSystem>().Update(renderer, assetStore);
}

void Game::Destroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
