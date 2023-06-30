#ifndef INPUTEVENT_H
#define INPUTEVENT_H

#include "Event.h"

enum InputButtonActionType
{
    INPUT_BUTTON_DEBUG_DRAW_PRESS,
    INPUT_BUTTON_DEBUG_DRAW_RELEASE,
    INPUT_BUTTON_UP_PRESS,
    INPUT_BUTTON_UP_RELEASE,
    INPUT_BUTTON_DOWN_PRESS,
    INPUT_BUTTON_DOWN_RELEASE,
    INPUT_BUTTON_LEFT_PRESS,
    INPUT_BUTTON_LEFT_RELEASE,
    INPUT_BUTTON_RIGHT_PRESS,
    INPUT_BUTTON_RIGHT_RELEASE,
    INPUT_BUTTON_SHOOT_PRESS,
    INPUT_BUTTON_SHOOT_RELEASE
};

class InputButtonEvent: public Event
{
public:
    InputButtonActionType actionType;

    InputButtonEvent(InputButtonActionType actionType): actionType(actionType) {}
};

class InputCursorEvent: public Event
{
public:
    int xPosition;
    int yPosition;

    InputCursorEvent(int xPosition, int yPosition): xPosition(xPosition), yPosition(yPosition) {}
};

#endif