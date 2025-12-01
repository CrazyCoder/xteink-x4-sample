#include "StatusBar.h"
#include "../UI.h"

UIStatusBar::UIStatusBar(int16_t x,
                         int16_t y,
                         int16_t width,
                         int16_t height)
    : x_(x),
      y_(y),
      w_(width),
      h_(height)
{
}

static void drawBatteryIcon(int16_t x, int16_t y, uint8_t pct) {
    // icon size
    const int16_t w = UI_STATUSBAR_BATTERYICON_W;
    const int16_t h = UI_STATUSBAR_BATTERYICON_H;
    const int16_t border = 2;

    // terminal
    UI_DISPLAY.drawRect(x + w, y + h/3, 3, h/3, GxEPD_BLACK);

    // outer body
    UI_DISPLAY.drawRect(x, y, w, h, GxEPD_BLACK);

    // inner fill area
    int16_t innerW = w - border * 2;
    int16_t innerH = h - border * 2;
    int16_t fillX = x + border;
    int16_t fillY = y + border;

    // compute fill width
    int16_t fillW = (innerW * pct) / 100;
    if (fillW < 0) {
        fillW = 0;
    }
    if (fillW > innerW) { 
        fillW = innerW;
    }

    // draw fill
    if (fillW > 0) {
        UI_DISPLAY.fillRect(fillX, fillY, fillW, innerH, GxEPD_BLACK);
    }
}

void UIStatusBar::draw(UIElement*) {
    UI_DISPLAY.fillRect(x_, y_, w_, h_, GxEPD_LIGHTGREY);
    if (UI_STATUSBAR_SHOW_PCT) {
        UI_DISPLAY.setFont(UI_FONT_SMALL);
    }

    int16_t iconX = x_ + w_ - UI_STATUSBAR_BATTERYICON_W - UI_MARGIN_S;
    int16_t iconY = y_ + (UI_STATUSBAR_HEIGHT - UI_STATUSBAR_BATTERYICON_H) / 2;

    auto& batteryMonitor = BatteryMonitor::getInstance();
    uint8_t pct = batteryMonitor.readPercentage();
    drawBatteryIcon(iconX, iconY, pct);
    if (UI_STATUSBAR_SHOW_PCT) {
        UI_DISPLAY.setCursor(x_ + UI_MARGIN_S, y_ + (h_ + UI_FONT_SMALL_SIZE) / 2);
        UI_DISPLAY.print(pct);
        UI_DISPLAY.print("%");
        if (batteryMonitor.isCharging()) {
            UI_DISPLAY.print(" (Charging)");
        }
    }

    // divider line below status bar
    UI_DISPLAY.drawRect(x_, y_ + h_ -2, w_, 2, GxEPD_BLACK);
}

bool UIStatusBar::onEvent(UIElement*, Button) {
    return false;
}

UIElement UIStatusBar::getElement() {
    return UIWidget::getElement(x_, y_, w_, h_);
}