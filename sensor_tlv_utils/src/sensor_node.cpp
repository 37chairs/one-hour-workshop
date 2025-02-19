#include "sensor_node.h"
#include "nvs_flash.h"
#include <cstring>

#define TAG "SENSOR_NODE"
static constexpr uint8_t BROADCAST_MAC[ESP_NOW_ETH_ALEN] = {0xFF, 0xFF, 0xFF,
                                                            0xFF, 0xFF, 0xFF};

void SensorNode::init() {
  ESP_LOGI(TAG, "Initializing NVS...");
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_LOGW(TAG, "Erasing NVS flash due to corruption...");
    ESP_ERROR_CHECK(nvs_flash_erase());
    ESP_ERROR_CHECK(nvs_flash_init());
  } else {
    ESP_ERROR_CHECK(ret);
  }

  ESP_LOGI(TAG, "Initializing WiFi...");
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_start());
  ESP_ERROR_CHECK(esp_wifi_set_protocol(
      WIFI_IF_STA, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N |
                       WIFI_PROTOCOL_LR));
  ESP_ERROR_CHECK(esp_wifi_set_channel(9, WIFI_SECOND_CHAN_NONE));

  ESP_LOGI(TAG, "Initializing ESP-NOW...");
  ESP_ERROR_CHECK(esp_now_init());
  ESP_ERROR_CHECK(esp_now_set_pmk((uint8_t *)"pmk1234567890123"));

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, BROADCAST_MAC, ESP_NOW_ETH_ALEN);
  peerInfo.channel = 9;
  peerInfo.ifidx = WIFI_IF_STA;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    ESP_LOGE(TAG, "Failed to add broadcast peer!");
  } else {
    ESP_LOGI(TAG, "Broadcast peer added successfully.");
  }
}

void SensorNode::send(SensorPayload &payload) {
  std::vector<uint8_t> encoded = payload.encode();
  esp_err_t err = esp_now_send(BROADCAST_MAC, encoded.data(), encoded.size());

  if (err == ESP_OK) {
    ESP_LOGI(TAG, "Packet sent successfully");
  } else {
    ESP_LOGE(TAG, "Packet send error: %s", esp_err_to_name(err));
  }
}

SensorNode::SensorNode(uint8_t id) : node_id(id) {}
