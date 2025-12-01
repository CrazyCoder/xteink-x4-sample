#pragma once
#include <stdint.h>
#include "Button.h"
#include <GxEPD2_BW.h>
#include "theme.h"

// Convenience macro for display type
extern GxEPD2_BW<GxEPD2_426_GDEQ0426T82,
                 GxEPD2_426_GDEQ0426T82::HEIGHT>* uiDisplayPtr;
#define UI_DISPLAY (*uiDisplayPtr)

// Forward declaration
template<typename DisplayT>
void uiInit(DisplayT* disp);

#ifndef UI_MAX_ELEMENTS
#define UI_MAX_ELEMENTS 16
#endif

struct UIElement;
class UIWidget;
enum Button;

// Callbacks
typedef void (*UI_DrawFn)(UIElement* self);
typedef bool (*UI_EventFn)(UIElement* self, Button b);

struct UIElement {
    int16_t x, y, w, h;

    UIWidget* widget;
    void (*draw)(UIElement* e);
    bool (*onEvent)(UIElement* e, Button b);
};

// --- Core API ---
void uiClear();
int  uiAddElement(const UIElement& e, bool focus = false);

void uiRenderFull();
void uiRenderPartial(int16_t x, int16_t y, int16_t w, int16_t h);

bool uiHandleButton(Button b);

void uiFocusNext();
void uiFocusPrev();
void uiSetFocus(int idx);
int  uiGetFocus();

// Low-level access
UIElement* uiElements();
int        uiElementCount();