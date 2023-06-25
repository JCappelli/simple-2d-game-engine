#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Events/EventBus.h"
#include "../Events/InputEvent.h"
#include <SDL2/SDL.h>

class InputSystem : public System
{
private:
    static const SDL_KeyCode DEBUG_DRAW_KEY = SDLK_BACKQUOTE;

public:
    InputSystem() = default;
    void PollKeyEvent(const SDL_Event& sdlEvent, std::unique_ptr<EventBus>& eventBus);
};

void InputSystem::PollKeyEvent(const SDL_Event& sdlEvent, std::unique_ptr<EventBus>& eventBus)
{
    switch (sdlEvent.key.keysym.sym)
    {
        case DEBUG_DRAW_KEY:
            if (sdlEvent.key.state == SDL_PRESSED)
            {
                eventBus->EmitEvent<InputButtonEvent>(INPUT_BUTTON_DEBUG_DRAW_PRESS);
            }
            else
            {
                eventBus->EmitEvent<InputButtonEvent>(INPUT_BUTTON_DEBUG_DRAW_RELEASE);
            }
            break;
        default:
            break;
    }
}
#endif