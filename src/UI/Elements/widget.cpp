#include "widget.h"
#include "../ui.h"

UIElement UIWidget::getElement(int16_t x, int16_t y, int16_t w, int16_t h) {
    UIElement e{};
    e.x = x;
    e.y = y;
    e.w = w;
    e.h = h;

    e.widget = this;

    // Bind draw callback
    e.draw = [](UIElement* ue) {
        ue->widget->draw(ue);
    };

    // Bind event callback
    e.onEvent = [](UIElement* ue, Button b) -> bool {
        return ue->widget->onEvent(ue, b);
    };

    return e;
}