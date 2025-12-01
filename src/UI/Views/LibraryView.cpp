#include "LibraryView.h"
#include "../UI.h"
#include "../../FileSystem.h"
#include "../../BatteryMonitor.h"

LibraryView::LibraryView()
    : statusBar_(nullptr), fileBrowser_(nullptr)
{
    // Status bar at top
    statusBar_ = new UIStatusBar(UI_SAFE_LEFT, UI_SAFE_TOP, UI_SAFE_WIDTH, UI_STATUSBAR_HEIGHT);
    addElement(statusBar_->getElement());

    // File browser below status bar
    fileBrowser_ = new UIFileBrowser("/", UI_SAFE_LEFT, UI_SAFE_TOP + UI_STATUSBAR_HEIGHT, UI_SAFE_WIDTH, UI_SAFE_HEIGHT - UI_STATUSBAR_HEIGHT);
    addElement(fileBrowser_->getElement(), true); // focus on browser
}

LibraryView::~LibraryView() {
    delete statusBar_;
    delete fileBrowser_;
}

void LibraryView::onEnter() {
    
}

void LibraryView::onExit() {
    
}