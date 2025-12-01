#pragma once
#include <stdint.h>
#include "../../Button.h"

struct UIElement;
class UIWidget {
public:
    virtual ~UIWidget() {}
    virtual void draw(UIElement* e) = 0;
    virtual bool onEvent(UIElement* e, Button b) = 0;
    UIElement getElement(int16_t x, int16_t y, int16_t w, int16_t h);
};