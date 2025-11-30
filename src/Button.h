#pragma once
#include <stdint.h>

// Button pins
#define BTN_GPIO1 1 // 4 buttons on ADC resistor ladder: Back, Confirm, Left, Right
#define BTN_GPIO2 2 // 2 buttons on ADC resistor ladder: Volume Up, Volume Down
#define BTN_GPIO3 3 // Power button (digital)

// Button ADC thresholds
const int BTN_THRESHOLD = 100; // Threshold tolerance
const int BTN_RIGHT_VAL = 3;
const int BTN_LEFT_VAL = 1470;
const int BTN_CONFIRM_VAL = 2655;
const int BTN_BACK_VAL = 3470;
const int BTN_VOLUME_DOWN_VAL = 3;
const int BTN_VOLUME_UP_VAL = 2205;

// Button enum
enum Button
{
  NONE = 0,
  RIGHT,
  LEFT,
  CONFIRM,
  BACK,
  VOLUME_UP,
  VOLUME_DOWN,
  POWER
};

// Power button timing
const unsigned long POWER_BUTTON_WAKEUP_MS = 1000; // Time required to confirm boot from sleep
const unsigned long POWER_BUTTON_SLEEP_MS = 1000;  // Time required to enter sleep mode

const char *getButtonName(Button btn);
Button GetPressedButton();