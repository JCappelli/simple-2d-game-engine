#ifndef INPUTEVENT_H
#define INPUTEVENT_H

#include "Event.h"

enum InputButtonActionType
{
    INPUT_BUTTON_DEBUG_DRAW_PRESS,
    INPUT_BUTTON_DEBUG_DRAW_RELEASE
};

class InputButtonEvent: public Event
{
public:
    InputButtonActionType actionType;

    InputButtonEvent(InputButtonActionType actionType): actionType(actionType) {}
};

#endif