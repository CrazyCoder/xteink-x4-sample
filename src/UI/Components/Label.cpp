#include "Label.h"
#include "../UI.h"

UILabel::UILabel(const char* text, int16_t x, int16_t y) : text_(text), x_(x), y_(y) {
}

void UILabel::setText(const char* t) {
    text_ = t;
}

void UILabel::draw(UIElement*) {
    UI_DISPLAY.setFont(UI_FONT_MAIN);
    UI_DISPLAY.setCursor(x_, y_);
    UI_DISPLAY.print(text_);
}

bool UILabel::onEvent(UIElement*, Button) {
    return false; // UILabel doesn’t handle events
}

UIElement UILabel::getElement() {
    return UIWidget::getElement(x_, y_, 300, 40);
}