#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_now.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include <string.h>

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_system.h"

#include "LovyanGFX.hpp"

#define TAG "RX_MAIN"

// ----------------- ESPNOW CONFIG -----------------
static const uint8_t PMK_KEY[] = "pmk1234567890123"; // Must be 16 bytes
static int msg_counter = 0;                          // Message counter

// ----------------- LCD CONFIGURATION -----------------
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

// ----------------- ESPNOW CALLBACK -----------------
static void espnow_recv_cb(const esp_now_recv_info_t *info, const uint8_t *data,
                           int len) {
  if (!info || !data || len <= 0) {
    ESP_LOGE(TAG, "Receive cb arg error");
    return;
  }

  msg_counter++;
  if (msg_counter > 1000) {
    msg_counter = 0;
  }

  ESP_LOGI(TAG, "Packet received from %02X:%02X:%02X:%02X:%02X:%02X, len: %d",
           info->src_addr[0], info->src_addr[1], info->src_addr[2],
           info->src_addr[3], info->src_addr[4], info->src_addr[5], len);

  // Log raw bytes to serial
  ESP_LOG_BUFFER_HEX(TAG, data, len);

  // Update the TFT display (unchanged)
  tft.clear();
  tft.setCursor(0, 0);
  tft.setTextSize(1);
  tft.printf("RX: %d   Delta: %.2f s\n", msg_counter,
             (double)esp_timer_get_time() / 10000000);
  tft.printf("DATA: ");
  for (int i = 0; i < len; i++) {
    tft.printf("%02X ", data[i]);
  }
  tft.printf("\nLength: %d", len);

  tft.display();
}

// ----------------- Wi-Fi + ESPNOW INIT -----------------
static void wifi_init(void) {
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_start());

  // Set Wi-Fi to use Long Range (802.11 LR) and disable power save
  ESP_ERROR_CHECK(esp_wifi_set_protocol(
      WIFI_IF_STA, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N |
                       WIFI_PROTOCOL_LR));
  ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));

  // Set ESP-NOW channel (must match transmitter)
  ESP_ERROR_CHECK(esp_wifi_set_channel(9, WIFI_SECOND_CHAN_NONE));
}

static void espnow_init(void) {
  ESP_ERROR_CHECK(esp_now_init());
  ESP_ERROR_CHECK(esp_now_register_recv_cb(espnow_recv_cb));
  ESP_ERROR_CHECK(esp_now_set_pmk(PMK_KEY));
}

// ----------------- MAIN -----------------
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
  tft.printf("Waiting for ESPNOW...");
  tft.display();

  // Initialize systems
  ESP_ERROR_CHECK(nvs_flash_init());
  wifi_init();
  espnow_init();

  ESP_LOGI(TAG, "Receiver initialized. Waiting for ESPNOW packets...");
}
