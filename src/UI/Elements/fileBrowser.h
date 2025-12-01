#pragma once
#include <vector>
#include <string>
#include "widget.h"
#include "../../FileSystem.h"

#define UI_FILEBROWSER_ROWHEIGHT 60
#define UI_FILEBROWSER_PAGINATION_BAR_HEIGHT 40

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

    int getSelectedIndex() const { return selectedIndex_; }
    const FileInfo* getSelectedFile() const;
    
private:
    int16_t x_, y_, w_, h_;
    std::vector<FileInfo> files_;
    int selectedIndex_ = 0;
    int previousSelectedIndex_ = -1;

    uint8_t rowsPerPage_;
    uint16_t currentPage_;
    uint16_t totalPages_;

    void updatePagination();

    void moveSelectionUp();
    void moveSelectionDown();
    void ensureSelectionVisible();
};