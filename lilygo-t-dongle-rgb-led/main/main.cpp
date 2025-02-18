#include "driver/spi_master.h"
#include "esp_log.h"
#include "freertos/task.h"
#include "led_strip_spi.h"

#include <esp_err.h>
#include <led_strip_spi_sk9822.h>

#define TAG "SPI-LED"

#define LED_STRIP_DATA_PIN GPIO_NUM_40
#define LED_STRIP_CLOCK_PIN GPIO_NUM_39
#define LED_SPI_HOST SPI2_HOST
#define LED_BRIGHTNESS 32 // 0-255

static spi_device_handle_t led_spi;
static led_strip_spi_t strip;

void setup_led_strip() {
  ESP_LOGI(TAG, "Initializing SPI bus for LED strip...");

  spi_bus_config_t buscfg = {
      .mosi_io_num = LED_STRIP_DATA_PIN,
      .miso_io_num = -1,
      .sclk_io_num = LED_STRIP_CLOCK_PIN,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      .max_transfer_sz = LED_STRIP_SPI_BUFFER_SIZE(1),
  };

  esp_err_t err = spi_bus_initialize(LED_SPI_HOST, &buscfg, SPI_DMA_CH_AUTO);
  if (err == ESP_ERR_INVALID_STATE) {
    ESP_LOGW(TAG, "SPI bus already initialized, skipping...");
  } else if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to initialize SPI bus: %s", esp_err_to_name(err));
    return;
  }

  spi_device_interface_config_t devcfg = {
      .mode = 3,
      .clock_speed_hz = 10000000, // 10MHz
      .spics_io_num = -1,         // No CS pin needed
      .queue_size = 1,
  };

  err = spi_bus_add_device(LED_SPI_HOST, &devcfg, &led_spi);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to add LED SPI device: %s", esp_err_to_name(err));
    return;
  }

  ESP_LOGI(TAG, "LED SPI device added.");

  strip = LED_STRIP_SPI_DEFAULT();
  strip.device_handle = led_spi;
  strip.length = 1; // Single LED
  strip.max_transfer_sz = LED_STRIP_SPI_BUFFER_SIZE(1);
  strip.clock_speed_hz = 10000000; // 10MHz

  err = led_strip_spi_init(&strip);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to initialize LED strip: %s", esp_err_to_name(err));
  }

  // Clear LED on startup
  led_strip_spi_flush(&strip);
}

void blink_rgb_led_task(void *pvParameter) {
  while (1) {
    static uint8_t counter = 0;
    uint32_t c;
    rgb_t color;
    esp_err_t err;

    // note: this is correct RGB order.
    c = 0x0000a0 << ((counter % 3) * 8);
    color.r = (c >> 16) & 0xff;
    color.g = (c >> 8) & 0xff;
    color.b = c & 0xff;
    color = rgb_scale(color, LED_BRIGHTNESS);
    ESP_LOGI(TAG, "r: 0x%02x g: 0x%02x b: 0x%02x", color.r, color.g, color.b);

    err = led_strip_spi_fill(&strip, 0, strip.length, color);
    if (err != ESP_OK) {
      ESP_LOGE(TAG, "led_strip_spi_fill(): %s", esp_err_to_name(err));
    }
    led_strip_spi_flush(&strip);
    counter += 1;

    vTaskDelay(pdMS_TO_TICKS(50)); // Adjust delay for smoother/slower transition
  }
}

extern "C" void app_main(void) {
  ESP_LOGI(TAG, "Device started, waiting 1 second before initializing LED");

  vTaskDelay(pdMS_TO_TICKS(1000));

  ESP_LOGI(TAG, "Installing LED Driver");
  esp_err_t err = led_strip_spi_install();
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "led_strip_spi_install(): %s", esp_err_to_name(err));
  }

  ESP_LOGI(TAG, "Waiting 1 second before setting up LED strip");
  vTaskDelay(pdMS_TO_TICKS(1000));

  setup_led_strip();
  ESP_LOGI(
      TAG,
      "LED strip setup complete, waiting 1 second before starting blink task");
  vTaskDelay(pdMS_TO_TICKS(1000));

  xTaskCreate(blink_rgb_led_task, "blink_rgb_led_task", 4096, NULL, 5, NULL);
}
