#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>
#include "../ECS/ECS.h"

class Game
{
    private:
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;
        bool isRunning = false;

        std::unique_ptr<Registry> registry;

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
        void Run();
        void Destroy();
};
#endif
 