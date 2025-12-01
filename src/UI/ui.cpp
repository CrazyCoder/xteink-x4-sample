#include "ui.h"

// A generic pointer to whichever display class was instantiated
static void* g_disp_void = nullptr;

// Local typed accessor
template<typename DisplayT>
inline DisplayT* uiDisplay() {
    return reinterpret_cast<DisplayT*>(g_disp_void);
}

static UIElement g_elements[UI_MAX_ELEMENTS];
static int g_count = 0;
static int g_focusIndex = -1;

// --------------------------------------------------
// uiInit — templated to capture display type
// --------------------------------------------------
GxEPD2_BW<GxEPD2_426_GDEQ0426T82,
          GxEPD2_426_GDEQ0426T82::HEIGHT>* uiDisplayPtr = nullptr;

template<typename DisplayT>
void uiInit(DisplayT* disp) {
    g_disp_void = disp;
    uiDisplayPtr = disp;
    g_count = 0;
    g_focusIndex = -1;
}

// Tell the compiler which instantiations are allowed (export)
template void uiInit<GxEPD2_BW<GxEPD2_426_GDEQ0426T82,
                               GxEPD2_426_GDEQ0426T82::HEIGHT>>
       (GxEPD2_BW<GxEPD2_426_GDEQ0426T82,
                  GxEPD2_426_GDEQ0426T82::HEIGHT>* disp);

// --------------------------------------------------
// Internal helpers
// --------------------------------------------------
static bool isFocusable(const UIElement& e) {
    return e.onEvent != nullptr;
}

static void clampFocus() {
    if (g_count == 0) { 
        g_focusIndex = -1; 
        return; 
    }
    if (g_focusIndex < 0) {
        g_focusIndex = 0;
    }
    if (g_focusIndex >= g_count) {
        g_focusIndex = g_count - 1;
    }
}

// --------------------------------------------------
// Core API
// --------------------------------------------------
void uiClear() {
    g_count = 0;
    g_focusIndex = -1;
}

int uiAddElement(const UIElement& e, bool focus) {
    if (g_count >= UI_MAX_ELEMENTS) {
        return -1;
    }

    g_elements[g_count] = e;

    if (focus || (g_focusIndex == -1 && isFocusable(e))) {
        g_focusIndex = g_count;
    }

    return g_count++;
}

void uiRenderFull() {
    if (!g_disp_void) {
        return;
    }

    UI_DISPLAY.setFullWindow();
    UI_DISPLAY.firstPage();
    do {
        UI_DISPLAY.fillScreen(GxEPD_WHITE);

        for (int i = 0; i < g_count; i++) {
            if (g_elements[i].draw) {
                g_elements[i].draw(&g_elements[i]);
            }
        }

    } while (UI_DISPLAY.nextPage());
}

void uiRenderPartial(int16_t x, int16_t y, int16_t w, int16_t h) {
    if (!g_disp_void) return;

    UI_DISPLAY.setPartialWindow(x, y, w, h);
    UI_DISPLAY.firstPage();
    do {
        UI_DISPLAY.fillScreen(GxEPD_WHITE);

        for (int i = 0; i < g_count; i++) {
            UIElement& e = g_elements[i];

            bool intersect =
                !(e.x + e.w < x || e.x > x + w ||
                  e.y + e.h < y || e.y > y + h);

            if (intersect && e.draw) {
                e.draw(&e);
            }
        }

    } while (UI_DISPLAY.nextPage());
}

bool uiHandleButton(Button b) {
    if (g_focusIndex < 0 || g_focusIndex >= g_count) {
        return false;
    }

    UIElement& e = g_elements[g_focusIndex];
    bool mustRedraw = e.onEvent ? e.onEvent(&e, b) : false;
    if (mustRedraw) {
        uiRenderPartial(e.x, e.y, e.w, e.h);
    }
    return mustRedraw;
}

void uiFocusNext() {
    if (g_count == 0) {
        return;
    }
    int start = g_focusIndex;
    int i = g_focusIndex;

    while (true) {
        i = (i + 1) % g_count;
        if (isFocusable(g_elements[i])) {
            g_focusIndex = i;
            return;
        }
        if (i == start) {
            return;
        }
    }
}

void uiFocusPrev() {
    if (g_count == 0) {
        return;
    }
    int start = g_focusIndex;
    int i = g_focusIndex;

    while (true) {
        i = (i - 1 + g_count) % g_count;
        if (isFocusable(g_elements[i])) {
            g_focusIndex = i;
            return;
        }
        if (i == start) {
            return;
        }
    }
}

void uiSetFocus(int idx) {
    if (idx < 0 || idx >= g_count) return;
    if (!isFocusable(g_elements[idx])) return;
    g_focusIndex = idx;
}

int uiGetFocus() { return g_focusIndex; }

UIElement* uiElements() { return g_elements; }
int uiElementCount() { return g_count; }