#include "Game.h"
#include <SDL2/SDL.h>
#include <iostream>

Game::Game()
{
    isRunning = false;
    std::cout << "Game Created" << std::endl;
}

Game::~Game(){}

void Game::Initialize()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "Failed to Init SDL" << std::endl;
        return;
    }

    window = SDL_CreateWindow(
        "Game", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        800, 
        600, 
        0);
    
    if (!window)
    {
        std::cerr << "Error Creating SDL window" << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        std::cerr << "Error Creating SDL renderer" << std::endl;
        return;
    }

    isRunning = true;
}

void Game::Run()
{
    while (isRunning)
    {
        ProcessInput();
        Render();
    }
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

    //TODO: Create an actual renderer

    SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
