// main example
#include "sensor_node.h"
#include "sensor_payload.h"
#include "sensor_tags.h"
#include <cstring>

#define NODE_ID 0x01
#define TEMP_SENSOR_PIN GPIO_NUM_4
#define TAG "TempNode"

class TemperatureNode : public SensorNode {
public:
  TemperatureNode() : SensorNode(NODE_ID) {}

  void loop() {
    ESP_LOGI(TAG, "Reading temperature...");
    float temp = readTemperature();                       // e.g., 47.8°F
    int16_t tempScaled = static_cast<int16_t>(temp * 10); // 478

    SensorPayload payload;
    payload.add(TEMPERATURE_TAG, int16ToBytes(tempScaled));

    ESP_LOGI(TAG, "Sending Payload... Tag: 0x%02X, Value: %d (%.1f°F)",
             TEMPERATURE_TAG, tempScaled, temp);
    std::vector<uint8_t> encoded = payload.encode();

    ESP_LOG_BUFFER_HEX_LEVEL(TAG, encoded.data(), encoded.size(), ESP_LOG_INFO);

    if (!encoded.empty()) {
      send(payload);
    } else {
      ESP_LOGW(TAG, "Skipping send: No valid sensor data.");
    }
  }

private:
  std::vector<uint8_t> int16ToBytes(int16_t value) {
    std::vector<uint8_t> bytes(2);
    bytes[0] = static_cast<uint8_t>(value >> 8);   // MSB
    bytes[1] = static_cast<uint8_t>(value & 0xFF); // LSB
    return bytes;
  }

  float readTemperature() {
    return 47.8f; // Replace with real sensor read
  }
};

extern "C" void app_main() {
  ESP_LOGI(TAG, "Starting Temperature Node with ID: %d", NODE_ID);
  vTaskDelay(pdMS_TO_TICKS(5000));

  TemperatureNode node;
  node.init();

  ESP_LOGI(TAG, "Completing initialization...");

  while (true) {
    node.loop();
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}
