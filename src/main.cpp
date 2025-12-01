#include <Arduino.h>
#include <SPI.h>
#include <FS.h>
#include <SD.h>
#include <string.h>

#include "image.h"
#include "BatteryMonitor.h"
#include "FileSystem.h"
#include "UI/ui.h"
#include "UI/Elements/label.h"
#include "UI/Elements/statusBar.h"
#include "UI/Elements/fileBrowser.h"

#define SPI_FQ 40000000
// Display SPI pins (custom pins for XteinkX4, not hardware SPI defaults)
#define EPD_SCLK 8  // SPI Clock
#define EPD_MOSI 10 // SPI MOSI (Master Out Slave In)
#define EPD_CS 21   // Chip Select
#define EPD_DC 4    // Data/Command
#define EPD_RST 5   // Reset
#define EPD_BUSY 6  // Busy

#define SD_SPI_CS   12
#define SD_SPI_MISO 7

static BatteryMonitor g_battery(BAT_GPIO0);

static int rawBat = 0;

static Button lastButton = NONE;
volatile Button currentPressedButton = NONE;

// Display command enum
enum DisplayCommand
{
  DISPLAY_NONE = 0,
  DISPLAY_INITIAL,
  DISPLAY_TEXT,
  DISPLAY_BATTERY,
  DISPLAY_SLEEP
};

volatile DisplayCommand displayCommand = DISPLAY_NONE;

// GxEPD2 display - Using GxEPD2_426_GDEQ0426T82
// Note: XteinkX4 has 4.26" 800x480 display
GxEPD2_BW<GxEPD2_426_GDEQ0426T82, GxEPD2_426_GDEQ0426T82::HEIGHT> display(
  GxEPD2_426_GDEQ0426T82(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));

// FreeRTOS task for non-blocking display updates
TaskHandle_t displayTaskHandle = NULL;

// Display update task running on separate core
void displayUpdateTask(void *parameter)
{
  while (1)
  {
    if (displayCommand != DISPLAY_NONE)
    {
      DisplayCommand cmd = displayCommand;
      displayCommand = DISPLAY_NONE;

      if (cmd == DISPLAY_INITIAL)
      {
        // Use full window for initial welcome screen
        display.setFullWindow();
        display.firstPage();
        do
        {
          display.fillScreen(GxEPD_WHITE);

          // Header font
          display.setFont(UI_FONT_MAIN);
          display.setCursor(20, 50);
          display.print("Xteink X4 Sample");

          // Button text with smaller font
          display.setFont(UI_FONT_MAIN);
          display.setCursor(20, 100);
          display.print(getButtonName(currentPressedButton));

          // Draw battery information
          // drawBatteryInfo();
          // Draw top 3 SD files below the battery block
          // drawSdTopFiles();

          // Draw image at bottom right
          int16_t imgWidth = 263;
          int16_t imgHeight = 280;
          int16_t imgMargin = 20;
          int16_t imgX = 480 - imgMargin - imgWidth;
          int16_t imgY = 800 - imgMargin - imgHeight;
          display.drawBitmap(imgX, imgY, dr_mario, imgWidth, imgHeight, GxEPD_BLACK);
        } while (display.nextPage());
      }
      else if (cmd == DISPLAY_TEXT)
      {
        // Use partial refresh for text updates
        display.setPartialWindow(0, 75, display.width(), 225);
        display.firstPage();
        do
        {
          display.fillScreen(GxEPD_WHITE);
          display.setFont(UI_FONT_MAIN);
          display.setCursor(20, 100);
          display.print(getButtonName(currentPressedButton));
          // drawBatteryInfo();
        } while (display.nextPage());
      }
      else if (cmd == DISPLAY_BATTERY)
      {
        // Use partial refresh for battery updates
        display.setPartialWindow(0, 135, display.width(), 200);
        display.firstPage();
        do
        {
          display.fillScreen(GxEPD_WHITE);
          // drawBatteryInfo();
        } while (display.nextPage());
      }
      else if (cmd == DISPLAY_SLEEP)
      {
        // Use full window for sleep screen
        display.setFullWindow();
        display.firstPage();
        do
        {
          display.fillScreen(GxEPD_WHITE);
          // Header font
          display.setFont(UI_FONT_MAIN);
          display.setCursor(120, 380);
          display.print("Sleeping...");
        } while (display.nextPage());
      }
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

// Verify long press on wake-up from deep sleep
void verifyWakeupLongPress()
{
  // Temporarily configure pin as digital input to check state
  pinMode(BTN_GPIO3, INPUT_PULLUP);

  unsigned long pressStart = millis();
  bool abortBoot = false;

  // Monitor button state for the duration
  while (millis() - pressStart < POWER_BUTTON_WAKEUP_MS)
  {
    // If button reads HIGH (released) before time is up
    if (digitalRead(BTN_GPIO3) == HIGH)
    {
      abortBoot = true;
      break;
    }
    delay(10);
  }

  if (abortBoot)
  {
    // Button released too early. Returning to sleep.
    // IMPORTANT: Re-arm the wakeup trigger before sleeping again
    esp_deep_sleep_enable_gpio_wakeup(1ULL << BTN_GPIO3, ESP_GPIO_WAKEUP_GPIO_LOW);
    esp_deep_sleep_start();
  }
  else
  {
    lastButton = POWER;
  }
}

// Enter deep sleep mode
void enterDeepSleep()
{
  displayCommand = DISPLAY_SLEEP;
  Serial.println("Power button released after a long press. Entering deep sleep.");
  delay(2000); // Allow Serial buffer to empty and display to update

  // Enable Wakeup on LOW (button press)
  esp_deep_sleep_enable_gpio_wakeup(1ULL << BTN_GPIO3, ESP_GPIO_WAKEUP_GPIO_LOW);

  // Enter Deep Sleep
  esp_deep_sleep_start();
}

FileSystem fileSys;
UIStatusBar statusBar(&g_battery, UI_SAFE_LEFT, UI_SAFE_TOP, UI_SAFE_WIDTH, UI_STATUSBAR_HEIGHT);
// UILabel labelElement("", &FreeMonoBold12pt7b, UI_SAFE_LEFT + UI_MARGIN_M, 120);
UIFileBrowser fileBrowser(UI_SAFE_LEFT, UI_SAFE_TOP + UI_STATUSBAR_HEIGHT, UI_SAFE_WIDTH, UI_SAFE_HEIGHT - UI_STATUSBAR_HEIGHT);

void refreshUI() {
    uiClear();
    uiAddElement(statusBar.getElement());
    // uiAddElement(labelElement.getElement());
    // labelElement.setText(label);

    auto files = fileSys.readFolder("/");
    fileBrowser.setFiles(files);
    uiAddElement(fileBrowser.getElement(), true);
    uiRenderFull();
}

void setup()
{
  // Check if boot was triggered by the Power Button (Deep Sleep Wakeup)
  // If triggered by RST pin or Battery insertion, this will be false, allowing normal boot.
  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_GPIO)
  {
    verifyWakeupLongPress();
  }

  Serial.begin(115200);

  // Wait for serial monitor
  unsigned long start = millis();
  while (!Serial && (millis() - start) < 3000) {
    delay(10);
  }

  if (Serial) {
    // delay for monitor to start reading
    delay(1000);
  }


  Serial.println("\n=================================");
  Serial.println("  xteink x4 sample");
  Serial.println("=================================");
  Serial.println();

  // Initialize button pins
  pinMode(BAT_GPIO0, INPUT);
  pinMode(BTN_GPIO1, INPUT);
  pinMode(BTN_GPIO2, INPUT);
  pinMode(BTN_GPIO3, INPUT_PULLUP); // Power button

  // Initialize SPI with custom pins
  SPI.begin(EPD_SCLK,SD_SPI_MISO, EPD_MOSI, EPD_CS);
  // Initialize display
  SPISettings spi_settings(SPI_FQ, MSBFIRST, SPI_MODE0);
  display.init(115200, true, 2, false, SPI, spi_settings);

  // SD Card Initialization
  fileSys.begin();

  // Setup display properties
  display.setRotation(3); // 270 degrees
  display.setTextColor(GxEPD_BLACK);  
  Serial.println("Display initialized");
  
  // Initialize UI
  uiInit(&display);
  refreshUI();

  // Draw initial welcome screen
  currentPressedButton = NONE;
  displayCommand = DISPLAY_INITIAL;

  // Create display update task on core 0 (main loop runs on core 1)
  // xTaskCreatePinnedToCore(displayUpdateTask,  // Task function
  //                         "DisplayUpdate",    // Task name
  //                         4096,               // Stack size
  //                         NULL,               // Parameters
  //                         1,                  // Priority
  //                         &displayTaskHandle, // Task handle
  //                         0                   // Core 0
  // );

  Serial.println("Display task created");
  Serial.println("Setup complete!\n");
}

#ifdef DEBUG_IO
void debugIO()
{
  // Log raw analog levels of BTN1 and BTN2 not more often than once per second
  rawBat = analogRead(BAT_GPIO0);
  int rawBtn1 = analogRead(BTN_GPIO1);
  int rawBtn2 = analogRead(BTN_GPIO2);
  int rawBtn3 = digitalRead(BTN_GPIO3);
  Serial.print("ADC BTN1=");
  Serial.print(rawBtn1);
  Serial.print("    BTN2=");
  Serial.print(rawBtn2);
  Serial.print("    BTN3=");
  Serial.print(rawBtn3);
  Serial.println("");

  // log battery info
  Serial.printf("== Battery (charging: %s) ==\n", g_battery.isCharging() ? "yes" : "no");
  Serial.print("Value from pin (raw/calibrated): ");
  Serial.print(rawBat);
  Serial.print(" / ");
  Serial.println(BatteryMonitor::millivoltsFromRawAdc(rawBat));
  Serial.print("Volts: ");
  Serial.println(g_battery.readVolts());
  Serial.print("Charge level: ");
  Serial.println(g_battery.readPercentage());
  Serial.println("");

  // SD card
}
#endif


void loop() {
  Button currentButton = GetPressedButton();

  // Detect button press (transition from NONE to a button)
  if (currentButton != NONE && lastButton == NONE) {
    const char *buttonName = getButtonName(currentButton);
    Serial.print("Button: ");
    Serial.println(buttonName);
    currentPressedButton = currentButton;
    displayCommand = DISPLAY_TEXT;

// #ifdef DEBUG_IO
//     debugIO();
// #endif

    uiHandleButton(currentPressedButton);

    if (currentButton == POWER) {
      unsigned long startTime = millis();
      // Wait for button release
      while (digitalRead(BTN_GPIO3) == LOW) {
        delay(50);
      }

      unsigned long currentTime = millis();
      // Power button long pressed => go to sleep
      if (currentTime - startTime > POWER_BUTTON_SLEEP_MS)
        enterDeepSleep();
    }
  }

  lastButton = currentButton;

  delay(50);
}
