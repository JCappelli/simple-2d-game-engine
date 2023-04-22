#include "Game.h"
#include "Logger.h"
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

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        Logger::Error("Error Creating SDL renderer");
        return;
    }

    isRunning = true;
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

glm::vec2 playerPosition = {100.0,100.0};
void Game::Update(float deltaTime)
{
    playerPosition.y += 30 * deltaTime;
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
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderClear(renderer);

    //Test Draw Rect
    SDL_Rect rectangle = {10, 10, 50, 100};
    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    SDL_RenderFillRect(renderer, &rectangle);

    //Test Draw Ship
    SDL_Surface* surface = IMG_Load("./assets/sprites/ships/ship_0000.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect dstRect = {
        static_cast<int>(playerPosition.x), 
        static_cast<int>(playerPosition.y), 
        32, 
        32};
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);

    SDL_DestroyTexture(texture);
    
    SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
