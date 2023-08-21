#include "Game.h"
#include "../Debugging/Logger.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/PlayerShootingComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/HealthComponent.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/PhysicsSystem.h"
#include "../Systems/InputSystem.h"
#include "../Systems/PlayerMovementSystem.h"
#include "../Systems/PlayerShootingSystem.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/ProjectileSystem.h"
#include "../Systems/RenderTextSystem.h"
#include "../Systems/ScriptExecutionSystem.h"
#include "../Debugging/DebugDrawCollidersSystem.h"
#include "../Debugging/DebugGUIRenderSystem.h"
#include "LevelLoader.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_sdlrenderer2.h>
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

    if (TTF_Init() != 0)
    {
        Logger::Error("Failed to Init SDL TTF");
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

    //Init Camera Rect
    int windowWidth = 0;
    int windowHeight = 0;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    cameraRect = {
        0,
        0,
        windowWidth,
        windowHeight
    };

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

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
    registry->AddSystem<PhysicsSystem>();
    registry->AddSystem<RenderSystem>();
    registry->AddSystem<AnimationSystem>();
    registry->AddSystem<InputSystem>();
    registry->AddSystem<PlayerMovementSystem>();
    registry->AddSystem<CameraMovementSystem>();
    registry->AddSystem<PlayerShootingSystem>();
    registry->AddSystem<ProjectileSystem>();
    registry->AddSystem<RenderTextSystem>();
    registry->AddSystem<ScriptExecutionSystem>();

    //Init Script System
    lua.open_libraries(sol::lib::base);
    lua.open_libraries(sol::lib::math);
    
    registry->GetSystem<ScriptExecutionSystem>().CreateLuaBindings(lua);
    
    //Debug Systems
    registry->AddSystem<DebugDrawCollidersSystem>();
    registry->AddSystem<DebugGUIRenderSystem>();

    //Init Global Assets
    assetStore->AddFont("picoFont", "./assets/fonts/pico-8.ttf", 8);

    LoadLevel();
}

void Game::LoadLevel()
{
    LevelLoader::LoadLevel(lua, "./assets/tilemaps/level1.lua", registry, assetStore, renderer);
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
    registry->GetSystem<DebugGUIRenderSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<PlayerMovementSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<PlayerShootingSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<ProjectileSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<ScriptExecutionSystem>().SubscribeToEvents(eventBus);

    //Update Systems
    registry->GetSystem<PhysicsSystem>().Update(deltaTime, eventBus);
    registry->GetSystem<AnimationSystem>().Update();
    registry->GetSystem<PlayerMovementSystem>().Update();
    registry->GetSystem<CameraMovementSystem>().Update(cameraRect);
    registry->GetSystem<PlayerShootingSystem>().Update(registry, cameraRect);
    registry->GetSystem<ProjectileSystem>().Update(deltaTime);
    registry->GetSystem<ScriptExecutionSystem>().Update(deltaTime);

    //Update entities (add/remove)
    registry->Update();
}

void Game::ProcessInput()
{
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent))
    {
        //ImGui event handling
        ImGui_ImplSDL2_ProcessEvent(&sdlEvent);

        //Game Loop Events
        switch (sdlEvent.type)
        {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                registry->GetSystem<InputSystem>().PollKeyEvent(sdlEvent, eventBus);
                break;
            case SDL_KEYUP:
                registry->GetSystem<InputSystem>().PollKeyEvent(sdlEvent, eventBus);
                break;
            case SDL_MOUSEMOTION:
                registry->GetSystem<InputSystem>().PollMouseEvent(sdlEvent, eventBus);
                break;
            case SDL_MOUSEBUTTONDOWN:
                registry->GetSystem<InputSystem>().PollMouseEvent(sdlEvent, eventBus);
                break;
            case SDL_MOUSEBUTTONUP:
                registry->GetSystem<InputSystem>().PollMouseEvent(sdlEvent, eventBus);
                break;
            default:
                break;
        }
    }
}

void Game::Render()
{
    SDL_SetRenderDrawColor(renderer, 118, 59, 54, 255);
    SDL_RenderClear(renderer);

    registry->GetSystem<RenderSystem>().Update(renderer, assetStore, cameraRect);
    registry->GetSystem<RenderTextSystem>().Update(renderer, assetStore, cameraRect);
    registry->GetSystem<DebugDrawCollidersSystem>().Update(renderer, cameraRect);
    registry->GetSystem<DebugGUIRenderSystem>().Update(registry, lua);

    SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
