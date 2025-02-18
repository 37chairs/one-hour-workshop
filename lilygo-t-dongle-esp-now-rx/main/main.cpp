#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_log.h"
#include "esp_now.h"

/* ----------------- ESPNOW CONFIG ----------------- */
static const char *TAG = "RX_MAIN";
static const uint8_t PMK_KEY[] = "pmk1234567890123"; // Must be 16 bytes

/* ----------------- ESPNOW CALLBACKS ----------------- */
static void espnow_recv_cb(const esp_now_recv_info_t *info, const uint8_t *data, int len)
{
    if (!info || !data || len <= 0) {
        ESP_LOGE(TAG, "Receive cb arg error");
        return;
    }

    // Log received packet
    ESP_LOGI(TAG, "Packet received from %02X:%02X:%02X:%02X:%02X:%02X, len: %d, Data: %.*s",
             info->src_addr[0], info->src_addr[1], info->src_addr[2],
             info->src_addr[3], info->src_addr[4], info->src_addr[5],
             len, len, data);
}

/* ------------- Wi-Fi + ESPNOW INIT ------------- */
static void wifi_init(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    // Set Wi-Fi to use Long Range (802.11 LR) and disable power save
    ESP_ERROR_CHECK(esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N | WIFI_PROTOCOL_LR));
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));

    // Set ESP-NOW channel (must match transmitter)
    ESP_ERROR_CHECK(esp_wifi_set_channel(9, WIFI_SECOND_CHAN_NONE));
}

static void espnow_init(void)
{
    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_recv_cb(espnow_recv_cb));
    ESP_ERROR_CHECK(esp_now_set_pmk(PMK_KEY));
}

extern "C" void app_main(void) {
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    wifi_init();
    espnow_init();

    ESP_LOGI(TAG, "Receiver initialized. Waiting for ESPNOW packets...");
}

