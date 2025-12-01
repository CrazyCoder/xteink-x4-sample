#include "View.h"

UIView::UIView() : focusIndex_(-1) {}
UIView::~UIView() {}

void UIView::onEnter() {}
void UIView::onExit() {}

void UIView::draw() {
    for (auto& e : elements_) {
        if (e.draw) {
            e.draw(&e);
        }
    }
}

void UIView::drawPartial(int16_t x, int16_t y, int16_t w, int16_t h) {
    for (auto& e : elements_) {
        if (!e.draw) {
            continue;
        }

        bool intersect =
            !(e.x + e.w < x || e.x > x + w ||
              e.y + e.h < y || e.y > y + h);
        if (intersect) {
            e.draw(&e);
        }
    }
}

bool UIView::handleButton(Button b) {
    if (focusIndex_ < 0 || focusIndex_ >= (int)elements_.size()) {
        return false;
    }

    UIElement& e = elements_[focusIndex_];
    bool redraw = e.onEvent ? e.onEvent(&e, b) : false;
    if (redraw) {
        UI::renderPartial(e.x, e.y, e.w, e.h);
    }
    return redraw;
}

void UIView::focusNext() {
    if (elements_.empty()) {
        return;
    }

    int start = focusIndex_;
    int i = focusIndex_;
    while (true) {
        i = (i + 1) % elements_.size();
        if (elements_[i].onEvent) { 
            focusIndex_ = i; return; 
        }
        if (i == start) {
            return;
        }
    }
}

void UIView::focusPrev() {
    if (elements_.empty()) return;

    int start = focusIndex_;
    int i = focusIndex_;
    while (true) {
        i = (i - 1 + elements_.size()) % elements_.size();
        if (elements_[i].onEvent) { 
            focusIndex_ = i; 
            return; 
        }
        if (i == start) { 
            return;
        }
    }
}

void UIView::setFocus(int idx) {
    if (idx < 0 || idx >= (int)elements_.size()) {
        return;
    }
    if (!elements_[idx].onEvent) {
        return;
    }
    focusIndex_ = idx;
}

int UIView::getFocus() const { 
    return focusIndex_; 
}

void UIView::addElement(UIElement e, bool focus) {
    elements_.push_back(e);
    if (focus || (focusIndex_ == -1 && e.onEvent)) {
        focusIndex_ = elements_.size() - 1;
    }
}

void UIView::clearElements() {
    elements_.clear();
    focusIndex_ = -1;
}