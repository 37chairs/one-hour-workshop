// sensor_payload.h
#ifndef SENSOR_PAYLOAD_H
#define SENSOR_PAYLOAD_H

#include "sensor_tlv_utils.h"
#include <cstdint>
#include <vector>

class SensorPayload {
  std::vector<SensorTLVUtils::TLV> tlvs;
  static constexpr size_t MAX_PAYLOAD_SIZE = 250;

public:
  void add(uint8_t type, std::vector<uint8_t> value);
  std::vector<uint8_t> encode(); // Adds packet length and CRC
};

#endif // SENSOR_PAYLOAD_H
