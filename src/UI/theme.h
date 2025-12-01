/**
 * Collection of global constants used everywhere in the UI
 */
#pragma once
#include <GxEPD2_BW.h>
#include <Fonts/FreeSerif18pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>

extern const GFXfont FONT_MAIN;
extern const GFXfont FONT_SMALL;

#define UI_FONT_MAIN        (&FONT_MAIN)
#define UI_FONT_MAIN_SIZE   18         
#define UI_FONT_SMALL       (&FONT_SMALL)
#define UI_FONT_SMALL_SIZE  12

#define UI_MARGIN_S     18
#define UI_MARGIN_M     36

// Pixels outside these coordinates don't get displayed on the X4 screen
#define UI_SAFE_LEFT    4
#define UI_SAFE_TOP     10
#define UI_SAFE_RIGHT   476
#define UI_SAFE_BOTTOM  795
#define UI_SAFE_WIDTH   (UI_SAFE_RIGHT - UI_SAFE_LEFT)
#define UI_SAFE_HEIGHT  (UI_SAFE_BOTTOM - UI_SAFE_TOP)
