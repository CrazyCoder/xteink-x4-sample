#pragma once
#include <vector>
#include <stdint.h>
#include "../UI.h"

struct UIElement;
enum Button;

class UIView {
public:
    UIView();
    virtual ~UIView();

    // View lifecycle
    virtual void onEnter();
    virtual void onExit();

    // Draw
    virtual void draw();
    virtual void drawPartial(int16_t x, int16_t y, int16_t w, int16_t h);

    // Button event handling
    bool handleButton(Button b);

    // Focus navigation
    void focusNext();
    void focusPrev();
    void setFocus(int idx);
    int getFocus() const;

    // Manage elements
    void addElement(UIElement e, bool focus = false);
    void clearElements();

private:
    std::vector<UIElement> elements_;
    int focusIndex_;
};