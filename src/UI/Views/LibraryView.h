#pragma once
#include "View.h"
#include "../Components/StatusBar.h"
#include "../Components/FileBrowser.h"

class LibraryView : public UIView {
public:
    LibraryView();
    virtual ~LibraryView();

    virtual void onEnter() override;
    virtual void onExit() override;

private:
    UIStatusBar* statusBar_;
    UIFileBrowser* fileBrowser_;
};