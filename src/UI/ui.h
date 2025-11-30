#pragma once
#include <stdint.h>
#include "Button.h"
#include <GxEPD2_BW.h>

// Pixels outside these coordinates don't get displayed on the X4 screen
#define UI_SAFE_LEFT    4
#define UI_SAFE_TOP     10
#define UI_SAFE_RIGHT   795
#define UI_SAFE_BOTTOM  476
#define UI_SAFE_WIDTH   (UI_SAFE_RIGHT - UI_SAFE_LEFT)
#define UI_SAFE_HEIGHT  (UI_SAFE_BOTTOM - UI_SAFE_TOP)

// Design system values
#define UI_MARGIN_S     8
#define UI_MARGIN_M     16

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
int  uiAddElement(const UIElement& e);

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