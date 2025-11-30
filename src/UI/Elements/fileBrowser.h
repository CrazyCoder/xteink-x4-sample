#pragma once
#include <vector>
#include <string>
#include "widget.h"
#include "../../FileSystem.h"

class UIFileBrowser : public UIWidget {
public:
    UIFileBrowser(int16_t x, int16_t y, int16_t width, int16_t height);

    // Set files to display
    void setFiles(const std::vector<FileInfo>& files);

    // Change page
    void nextPage();
    void prevPage();

    // Draw overrides
    void draw(UIElement* e) override;
    bool onEvent(UIElement* e, Button b) override;

    UIElement getElement();

private:
    int16_t x_, y_, w_, h_;
    std::vector<FileInfo> files_;

    uint8_t rowsPerPage_;
    uint16_t currentPage_;
    uint16_t totalPages_;

    void updatePagination();
};