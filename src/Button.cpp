#include <Arduino.h>
#include <SPI.h>

#include "Button.h"

Button GetPressedButton()
{
    int btn1 = analogRead(BTN_GPIO1);
    int btn2 = analogRead(BTN_GPIO2);

    // Check BTN_GPIO3 (Power button) - digital read
    if (digitalRead(BTN_GPIO3) == LOW)
    {
        return POWER;
    }
    // Check BTN_GPIO1 (4 buttons on resistor ladder)
    if (btn1 < BTN_RIGHT_VAL + BTN_THRESHOLD)
    {
        return RIGHT;
    }
    else if (btn1 < BTN_LEFT_VAL + BTN_THRESHOLD)
    {
        return LEFT;
    }
    else if (btn1 < BTN_CONFIRM_VAL + BTN_THRESHOLD)
    {
        return CONFIRM;
    }
    else if (btn1 < BTN_BACK_VAL + BTN_THRESHOLD)
    {
        return BACK;
    }

    // Check BTN_GPIO2 (2 buttons on resistor ladder)
    if (btn2 < BTN_VOLUME_DOWN_VAL + BTN_THRESHOLD)
    {
        return VOLUME_DOWN;
    }
    else if (btn2 < BTN_VOLUME_UP_VAL + BTN_THRESHOLD)
    {
        return VOLUME_UP;
    }

    return NONE;
}

// Get button name as string
const char *getButtonName(Button btn)
{
  switch (btn)
  {
  case NONE:
    return "Press any button";
  case RIGHT:
    return "RIGHT pressed!";
  case LEFT:
    return "LEFT pressed!";
  case CONFIRM:
    return "CONFIRM pressed!";
  case BACK:
    return "BACK pressed!";
  case VOLUME_UP:
    return "VOLUME UP pressed!";
  case VOLUME_DOWN:
    return "VOLUME DOWN pressed!";
  case POWER:
    return "POWER pressed!";
  default:
    return "";
  }
}