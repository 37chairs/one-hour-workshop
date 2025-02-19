# Sensor TLV Utils

This library provides a structure and interface for configuring ESP-NOW
sensor nodes and sending data using TLV (Type-Length-Value) encoding. 

```
sensor_tlv_utils/
├── include/
│   ├── sensor_tlv_utils.h       # TLV encoding/decoding utilities
│   ├── sensor_node.h            # Standardized sensor node class
│   ├── sensor_payload.h         # TLV payload helper
├── src/
│   ├── sensor_tlv_utils.cpp     # Implements TLV encoding/decoding
│   ├── sensor_node.cpp          # Implements ESP-NOW setup & sending logic
│   ├── sensor_payload.cpp       # Implements TLV payload builder
├── CMakeLists.txt               # Component definition
```

## Example Sensor node

```cpp
#include "sensor_node.h"
#include "sensor_payload.h"
#include "driver/gpio.h"

#define NODE_ID 0x01 // Unique per node
#define TEMP_SENSOR_PIN GPIO_NUM_4

class TemperatureNode : public SensorNode {
public:
    TemperatureNode() : SensorNode(NODE_ID) {}

    void loop() {
        float temp = readTemperature();
        SensorPayload payload;
        payload.add(0x01, {static_cast<uint8_t>(temp)}); // TLV tag for temperature
        send(payload);
    }

private:
    float readTemperature() {
        // Fake sensor reading (Replace with real GPIO reading)
        return 22.5f;
    }
};

extern "C" void app_main() {
    TemperatureNode node;
    node.init();

    while (true) {
        node.loop();
        vTaskDelay(pdMS_TO_TICKS(5000)); // Adjust timing per node
    }
}
```
