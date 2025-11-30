#include <Fonts/FreeMonoBold12pt7b.h>
#include "fileBrowser.h"
#include "../ui.h"

UIFileBrowser::UIFileBrowser(int16_t x, int16_t y, int16_t width, int16_t height)
    : x_(x), y_(y), w_(width), h_(height),
      currentPage_(0)
{
    rowsPerPage_ = h_ / 30; // each row 30px tall
}

void UIFileBrowser::setFiles(const std::vector<FileInfo>& files) {
    files_ = files;
    updatePagination();
}

void UIFileBrowser::updatePagination() {
    totalPages_ = (files_.size() + rowsPerPage_ - 1) / rowsPerPage_;
    if (currentPage_ >= totalPages_) currentPage_ = totalPages_ - 1;
}

void UIFileBrowser::nextPage() {
    if (currentPage_ + 1 < totalPages_) currentPage_++;
}

void UIFileBrowser::prevPage() {
    if (currentPage_ > 0) currentPage_--;
}

bool UIFileBrowser::onEvent(UIElement* e, Button b) {
    switch (b) {
        case VOLUME_DOWN: nextPage(); return true;
        case VOLUME_UP: prevPage(); return true;
        case RIGHT: nextPage(); return true;
        case LEFT: prevPage(); return true;
        default: return false;
    }
}

void UIFileBrowser::draw(UIElement* e) {
    
    UI_DISPLAY.setFont(&FreeMonoBold12pt7b);

    int16_t rowH = 30;
    int16_t startY = y_;

    uint16_t startIdx = currentPage_ * rowsPerPage_;
    uint16_t endIdx = startIdx + rowsPerPage_;
    if (endIdx > files_.size()) endIdx = files_.size();

    for (uint16_t i = startIdx; i < endIdx; i++) {
        const auto& f = files_[i];
        int16_t rowY = startY + (i - startIdx) * rowH;

        // icon rectangle
        UI_DISPLAY.drawRect(x_ + 2, rowY + 8, 14, 14, GxEPD_BLACK);

        // file name
        UI_DISPLAY.setCursor(x_ + 20, rowY + 20);
        UI_DISPLAY.print(f.name.c_str());
    }

    // pagination at bottom
    char pageBuf[32];
    snprintf(pageBuf, sizeof(pageBuf), "Page %u / %u", currentPage_ + 1, totalPages_);
    UI_DISPLAY.setCursor(x_ + 10, y_ + h_ - 10);
    UI_DISPLAY.print(pageBuf);
}

UIElement UIFileBrowser::getElement() {
    return UIWidget::getElement(x_, y_, w_, h_);
}