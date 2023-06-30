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
    static const SDL_KeyCode UP_KEY = SDLK_w;
    static const SDL_KeyCode DOWN_KEY = SDLK_s;
    static const SDL_KeyCode LEFT_KEY = SDLK_a;
    static const SDL_KeyCode RIGHT_KEY = SDLK_d;

public:
    InputSystem() = default;
    void PollKeyEvent(const SDL_Event& sdlEvent, std::unique_ptr<EventBus>& eventBus);
    void PollMouseEvent(const SDL_Event& sdlEvent, std::unique_ptr<EventBus>& eventBus);
};

void InputSystem::PollMouseEvent(const SDL_Event& sdlEvent, std::unique_ptr<EventBus>& eventBus)
{
    int x = 0;
    int y = 0;
    switch (sdlEvent.type)
    {
    case SDL_MOUSEMOTION:
        SDL_GetMouseState(&x, &y);
        eventBus->EmitEvent<InputCursorEvent>(x, y);
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (sdlEvent.button.button == SDL_BUTTON_LEFT)
        {
            eventBus->EmitEvent<InputButtonEvent>(INPUT_BUTTON_SHOOT_PRESS);
        }
        break;
    case SDL_MOUSEBUTTONUP:
        if (sdlEvent.button.button == SDL_BUTTON_LEFT)
        {
            eventBus->EmitEvent<InputButtonEvent>(INPUT_BUTTON_SHOOT_RELEASE);
        }
        break;
    default:
        break;
    }
}

void InputSystem::PollKeyEvent(const SDL_Event& sdlEvent, std::unique_ptr<EventBus>& eventBus)
{
    if (sdlEvent.key.repeat != 0)//ignore repeat events
        return;

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
        case UP_KEY:
            if (sdlEvent.key.state == SDL_PRESSED)
            {
                eventBus->EmitEvent<InputButtonEvent>(INPUT_BUTTON_UP_PRESS);
            }
            else
            {
                eventBus->EmitEvent<InputButtonEvent>(INPUT_BUTTON_UP_RELEASE);
            }
            break;
        case DOWN_KEY:
            if (sdlEvent.key.state == SDL_PRESSED)
            {
                eventBus->EmitEvent<InputButtonEvent>(INPUT_BUTTON_DOWN_PRESS);
            }
            else
            {
                eventBus->EmitEvent<InputButtonEvent>(INPUT_BUTTON_DOWN_RELEASE);
            }
            break;
        case LEFT_KEY:
            if (sdlEvent.key.state == SDL_PRESSED)
            {
                eventBus->EmitEvent<InputButtonEvent>(INPUT_BUTTON_LEFT_PRESS);
            }
            else
            {
                eventBus->EmitEvent<InputButtonEvent>(INPUT_BUTTON_LEFT_RELEASE);
            }
            break;
        case RIGHT_KEY:
            if (sdlEvent.key.state == SDL_PRESSED)
            {
                eventBus->EmitEvent<InputButtonEvent>(INPUT_BUTTON_RIGHT_PRESS);
            }
            else
            {
                eventBus->EmitEvent<InputButtonEvent>(INPUT_BUTTON_RIGHT_RELEASE);
            }
            break;
        default:
            break;
    }
}
#endif