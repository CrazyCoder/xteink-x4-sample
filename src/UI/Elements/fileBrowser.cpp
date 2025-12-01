#include "fileBrowser.h"
#include "../ui.h"

UIFileBrowser::UIFileBrowser(int16_t x, int16_t y, int16_t width, int16_t height)
    : x_(x), y_(y), w_(width), h_(height),
      currentPage_(0)
{
    rowsPerPage_ = h_ / UI_FILEBROWSER_ROWHEIGHT;
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

void UIFileBrowser::draw(UIElement* e) {
    UI_DISPLAY.setFont(UI_FONT_MAIN);

    int16_t rowH = UI_FILEBROWSER_ROWHEIGHT;
    int16_t startY = y_;

    uint16_t startIdx = currentPage_ * rowsPerPage_;
    uint16_t endIdx = startIdx + rowsPerPage_;
    if (endIdx > files_.size()) {
        endIdx = files_.size();
    }

    for (uint16_t i = startIdx; i < endIdx; i++) {
        const auto& f = files_[i];
        int16_t rowY = startY + (i - startIdx) * rowH;
        
        bool isSelected = (i == selectedIndex_);
        if (isSelected) {
            UI_DISPLAY.fillRect(x_, rowY, w_, UI_FILEBROWSER_ROWHEIGHT, GxEPD_BLACK);
            UI_DISPLAY.setTextColor(GxEPD_WHITE);
        } else {
            UI_DISPLAY.fillRect(x_, rowY, w_, UI_FILEBROWSER_ROWHEIGHT, GxEPD_WHITE);
            UI_DISPLAY.setTextColor(GxEPD_BLACK);
        }

        // icon rectangle
        UI_DISPLAY.drawRect(x_ + UI_MARGIN_S, rowY + 5, UI_FILEBROWSER_ROWHEIGHT - 10, UI_FILEBROWSER_ROWHEIGHT - 10, isSelected ? GxEPD_WHITE : GxEPD_BLACK);

        // file name
        UI_DISPLAY.setCursor(x_ + UI_MARGIN_S + UI_FILEBROWSER_ROWHEIGHT - 10 + UI_MARGIN_S, rowY + (UI_FILEBROWSER_ROWHEIGHT + UI_FONT_MAIN_SIZE) / 2);
        UI_DISPLAY.print(f.name.c_str());

        // divider
        UI_DISPLAY.drawFastHLine(x_, rowY + UI_FILEBROWSER_ROWHEIGHT - 1, w_, GxEPD_BLACK);
    }

    // pagination at bottom
    UI_DISPLAY.setFont(UI_FONT_SMALL);
    UI_DISPLAY.setTextColor(GxEPD_BLACK);
    UI_DISPLAY.drawRect(x_, y_ + h_ - UI_FILEBROWSER_PAGINATION_BAR_HEIGHT, w_, 2, GxEPD_BLACK);
    char pageBuf[32];
    snprintf(pageBuf, sizeof(pageBuf), "Page %u / %u", currentPage_ + 1, totalPages_);
    UI_DISPLAY.setCursor(x_ + UI_MARGIN_S, y_ + h_ - (UI_FILEBROWSER_PAGINATION_BAR_HEIGHT - UI_FONT_SMALL_SIZE) / 2);
    UI_DISPLAY.print(pageBuf);
}

UIElement UIFileBrowser::getElement() {
    return UIWidget::getElement(x_, y_, w_, h_);
}

const FileInfo* UIFileBrowser::getSelectedFile() const {
    if (selectedIndex_ < 0 || selectedIndex_ >= (int)files_.size()) return nullptr;
    return &files_[selectedIndex_];
}

void UIFileBrowser::moveSelectionUp() {
    if (selectedIndex_ > 0) {
        selectedIndex_--;
        ensureSelectionVisible();
    }
}

void UIFileBrowser::moveSelectionDown() {
    if (selectedIndex_ + 1 < (int)files_.size()) {
        selectedIndex_++;
        ensureSelectionVisible();
    }
}

void UIFileBrowser::ensureSelectionVisible() {
    uint16_t startIdx = currentPage_ * rowsPerPage_;
    uint16_t endIdx = startIdx + rowsPerPage_;

    if (selectedIndex_ < (int)startIdx) {
        currentPage_ = selectedIndex_ / rowsPerPage_;
    } else if (selectedIndex_ >= (int)endIdx) {
        currentPage_ = selectedIndex_ / rowsPerPage_;
    }
}

bool UIFileBrowser::onEvent(UIElement* e, Button b) {
    switch (b) {
        case VOLUME_UP:
            moveSelectionUp();
            return true;

        case VOLUME_DOWN:
            moveSelectionDown();
            return true;

        case LEFT:
            prevPage();
            return true;

        case RIGHT:
            nextPage();
            return true;

        default:
            return false;
    }
}