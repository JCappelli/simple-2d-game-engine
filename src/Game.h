#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>

class Game
{
    private:
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;
        bool isRunning = false;

        void ProcessInput();
        void Render();

    public:
        Game();
        ~Game();

        void Initialize();
        void Run();
        void Destroy();
};
#endif
