#pragma once
#include <GxEPD2_BW.h>
#include "Button.h"
#include "Theme.h"

// Convenience macro for display type
using GxEPD2_BW_426 = GxEPD2_BW<GxEPD2_426_GDEQ0426T82,
                 GxEPD2_426_GDEQ0426T82::HEIGHT>;
extern GxEPD2_BW_426* uiDisplayPtr;
#define UI_DISPLAY (*uiDisplayPtr)

// Template accessor for typed pointer
template<typename DisplayT>
DisplayT* uiDisplay();

// Forward declarations
class UIView; 
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


class UI {
public:
    static void init(GxEPD2_BW_426* display);

    // Clear screen
    static void clear();

    // Render active view
    static void renderFull();
    static void renderPartial(int16_t x, int16_t y, int16_t w, int16_t h);
    static void renderBlock(int16_t x, int16_t y, int16_t w, int16_t h, const std::function<void()>& fn);

    // Set active view
    static void setActiveView(UIView* view);

    // Forward button event to active view
    static bool handleButton(Button b);

private:
    UI() = default;
};