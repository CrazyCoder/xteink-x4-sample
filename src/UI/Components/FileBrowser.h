#pragma once
#include <vector>
#include <string>
#include "Widget.h"
#include "../../FileSystem.h"

#define UI_FILEBROWSER_ROWHEIGHT 59
#define UI_FILEBROWSER_PAGINATION_BAR_HEIGHT 42

class UIFileBrowser : public UIWidget {
public:
    UIFileBrowser(std::string path, int16_t x, int16_t y, int16_t width, int16_t height);

    // Nav
    void setPath(std::string path);
    void nextPage();
    void prevPage();
    void refresh();

    // Draw overrides
    void draw(UIElement* e) override;
    void drawRow(uint16_t i);
    void renderRows(uint16_t startIdx, uint16_t endIdx);
    bool onEvent(UIElement* e, Button b) override;

    UIElement getElement();

    int getSelectedIndex() const { return selectedIndex_; }
    const FileInfo* getSelectedFile() const;
    
private:
    int16_t x_, y_, w_, h_;

    std::string path_;
    std::vector<FileInfo> files_;
    int selectedIndex_ = 0;
    int previousSelectedIndex_ = -1;

    uint8_t rowsPerPage_;
    uint16_t currentPage_;
    uint16_t totalPages_;

    void setFiles(const std::vector<FileInfo>& files);
    void updatePagination();
    void openSelected();
    void openParentDirectory();

    bool moveSelectionUp();
    bool moveSelectionDown();
    bool ensureSelectionVisible();
};