// sensor_node.h
#ifndef SENSOR_NODE_H
#define SENSOR_NODE_H

#include "esp_log.h"
#include "esp_now.h"
#include "esp_wifi.h"
#include "sensor_payload.h"

class SensorNode {
protected:
  uint8_t node_id;

public:
  explicit SensorNode(uint8_t id);
  void init();
  void send(SensorPayload &payload);
};

#endif // SENSOR_NODE_H
