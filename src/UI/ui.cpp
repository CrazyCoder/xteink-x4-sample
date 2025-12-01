#include "UI.h"
#include "Views/View.h"

#include "UI.h"

// Generic pointer to any display type (template accessor)
static void* g_disp_void = nullptr;
GxEPD2_BW_426* uiDisplayPtr = nullptr;

// Active view
static UIView* g_activeView = nullptr;

template<typename DisplayT>
DisplayT* uiDisplay() {
    return reinterpret_cast<DisplayT*>(g_disp_void);
}

void UI::init(GxEPD2_BW_426* display) {
    uiDisplayPtr = display;
    g_disp_void = display;
    g_activeView = nullptr;
}

void UI::clear() {
    if (!uiDisplayPtr) return;
    uiDisplayPtr->fillScreen(GxEPD_WHITE);
    uiDisplayPtr->firstPage();
    do {} while (uiDisplayPtr->nextPage());
}

void UI::setActiveView(UIView* view) {
    if (g_activeView) g_activeView->onExit();
    g_activeView = view;
    if (g_activeView) g_activeView->onEnter();

    clear();
    renderFull();
}

bool UI::handleButton(Button b) {
    if (g_activeView) return g_activeView->handleButton(b);
    return false;
}

void UI::renderFull() {
    if (!uiDisplayPtr || !g_activeView) return;
    uiDisplayPtr->setFullWindow();
    uiDisplayPtr->firstPage();
    do {
        g_activeView->draw();
    } while (uiDisplayPtr->nextPage());
}

void UI::renderPartial(int16_t x, int16_t y, int16_t w, int16_t h) {
    if (!uiDisplayPtr || !g_activeView) return;
    uiDisplayPtr->setPartialWindow(x, y, w, h);
    uiDisplayPtr->firstPage();
    do {
        g_activeView->drawPartial(x, y, w, h);
    } while (uiDisplayPtr->nextPage());
}

void UI::renderBlock(int16_t x, int16_t y, int16_t w, int16_t h, const std::function<void()>& fn) {
    if (!uiDisplayPtr) return;

    uiDisplayPtr->setPartialWindow(x, y, w, h);
    uiDisplayPtr->firstPage();
    do {
        fn();
    } while (uiDisplayPtr->nextPage());
}