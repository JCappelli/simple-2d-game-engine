#ifndef TEXTLABELCOMPONENT_H
#define TEXTLABELCOMPONENT_H

#include <glm.hpp>
#include <string>
#include <SDL.h>

struct TextLabelComponent
{
    glm::vec2 position;
    std::string text;
    std::string assetId;
    SDL_Color color;

    TextLabelComponent(glm::vec2 position = glm::vec2(0,0), std::string text = "", std::string assetId = "", SDL_Color color = SDL_Color(), bool isFixed = false)
    {
        this->position = position;
        this->text = text;
        this->assetId = assetId;
        this->color = color;
    }
};

#endif