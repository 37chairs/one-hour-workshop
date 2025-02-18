#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "LovyanGFX.hpp"

#include <esp_err.h>

#define TAG "T-Dongle-S3"

// Display (ST7735s) hardware configuration:
#define DISPLAY_RST 1
#define DISPLAY_DC 2
#define DISPLAY_MOSI 3
#define DISPLAY_CS 4
#define DISPLAY_SCLK 5
#define DISPLAY_LEDA 38
#define DISPLAY_MISO -1
#define DISPLAY_BUSY -1
#define DISPLAY_WIDTH 160
#define DISPLAY_HEIGHT 80

// ==================== LCD CONFIGURATION ====================
class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ST7735S _panel_instance;
  lgfx::Bus_SPI _bus_instance;
  lgfx::Light_PWM _light_instance;

public:
  LGFX(void) {
    { // Configure SPI Bus
      auto cfg = _bus_instance.config();
      cfg.spi_mode = 0;
      cfg.freq_write = 27000000;
      cfg.freq_read = 16000000;
      cfg.pin_sclk = DISPLAY_SCLK;
      cfg.pin_mosi = DISPLAY_MOSI;
      cfg.pin_miso = DISPLAY_MISO;
      cfg.pin_dc = DISPLAY_DC;
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }
    { // Configure Display Panel
      auto cfg = _panel_instance.config();
      cfg.pin_cs = DISPLAY_CS;
      cfg.pin_rst = DISPLAY_RST;
      cfg.pin_busy = DISPLAY_BUSY;
      cfg.panel_width = DISPLAY_HEIGHT;
      cfg.panel_height = DISPLAY_WIDTH;
      cfg.offset_rotation = 1;
      cfg.readable = true;
      cfg.invert = true;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = true;
      cfg.offset_x = 26;
      cfg.offset_y = 1;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits = 1;
      cfg.memory_width = 132;
      cfg.memory_height = 160;
      _panel_instance.config(cfg);
    }
    {
      auto cfg = _light_instance.config();
      cfg.pin_bl = DISPLAY_LEDA;
      cfg.invert = true;
      cfg.freq = 12000;
      cfg.pwm_channel = 7;
      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);
    }
    setPanel(&_panel_instance);
  }
};

static LGFX tft;

// ==================== MAIN ====================
extern "C" void app_main(void) {
  ESP_LOGI(TAG, "Initializing LCD...");

  // Enable backlight
  gpio_reset_pin(static_cast<gpio_num_t>(DISPLAY_LEDA));
  gpio_set_direction(static_cast<gpio_num_t>(DISPLAY_LEDA), GPIO_MODE_OUTPUT);
  gpio_set_level(static_cast<gpio_num_t>(DISPLAY_LEDA), 1);
  ESP_LOGI(TAG, "Backlight set HIGH, waiting...");
  vTaskDelay(pdMS_TO_TICKS(2000));

  // Initialize LCD
  tft.init();
  vTaskDelay(pdMS_TO_TICKS(500));

  // Draw Initial Display
  tft.setRotation(0);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 0);
  tft.println("Hello, ESP-IDF!");
  tft.display();

  int counter = 0;
  while (true) {
    tft.clear();
    tft.setCursor(0, 0);
    tft.printf("Counter: \n> %d", counter++);
    tft.display();

    if (counter > 1000) {
      counter = 0;
    }

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
