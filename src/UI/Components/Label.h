#pragma once
#include "Widget.h"
#include <GxEPD2_BW.h>

class UILabel : public UIWidget {
public:
    UILabel(const char* text, int16_t x, int16_t y);

    void setText(const char* t);

    void draw(UIElement* e) override;
    bool onEvent(UIElement* e, Button b) override;

    UIElement getElement();

private:
    const char* text_;
    int16_t x_, y_;
};