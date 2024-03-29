#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../Events/EventBus.h"
#include <sol.hpp>

class Game
{
    private:
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;
        SDL_Rect cameraRect = {};
        bool isRunning = false;

        sol::state lua;

        std::unique_ptr<Registry> registry;
        std::unique_ptr<AssetStore> assetStore;
        std::unique_ptr<EventBus> eventBus;

        void ProcessInput();
        void Update(float deltaTime);
        void Render();

    public:
        const int TARGET_FPS = 60;
        const Uint64 TARGET_MILLISECS_PER_FRAME = 1000 / static_cast<Uint64>(TARGET_FPS);

        Game();
        ~Game();

        void Initialize();
        void Setup();
        void LoadLevel();
        void Run();
        void Destroy();
};
#endif
 