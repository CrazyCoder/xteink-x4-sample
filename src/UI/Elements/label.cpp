#include "label.h"
#include "../ui.h"

UILabel::UILabel(const char* text, const GFXfont* font, int16_t x, int16_t y) : text_(text), font_(font), x_(x), y_(y) {
}

void UILabel::setText(const char* t) {
    text_ = t;
}

void UILabel::draw(UIElement*) {
    if (font_) {
        UI_DISPLAY.setFont(font_);
    }
    UI_DISPLAY.setCursor(x_, y_);
    UI_DISPLAY.print(text_);
}

bool UILabel::onEvent(UIElement*, Button) {
    return false; // UILabel doesn’t handle events
}

UIElement UILabel::getElement() {
    return UIWidget::getElement(x_, y_, 300, 40);
}