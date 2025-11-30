#pragma once
#include "widget.h"
#include "../../BatteryMonitor.h"

#define UI_STATUSBAR_HEIGHT 40
#define UI_STATUSBAR_BATTERYICON_W 36
#define UI_STATUSBAR_BATTERYICON_H 20

class UIStatusBar : public UIWidget {
public:
    UIStatusBar(const BatteryMonitor* batteryMonitor,
                int16_t x,
                int16_t y,
                int16_t width,
                int16_t height);

    void draw(UIElement* e) override;
    bool onEvent(UIElement*, Button) override;
    UIElement getElement();

private:
    const BatteryMonitor* batteryMonitor_;
    int16_t x_, y_, w_, h_;
};