// sensor_payload.cpp
#include "sensor_payload.h"
#include "esp_log.h"
#include "sensor_tags.h"

#define TAG "SENSOR_PAYLOAD"

// Simple CRC-8 calculation (using CRC-8-ATM polynomial 0x07)
static uint8_t calculateCRC8(const std::vector<uint8_t> &data) {
  uint8_t crc = 0x00;
  for (uint8_t byte : data) {
    crc ^= byte;
    for (int i = 0; i < 8; i++) {
      if (crc & 0x80) {
        crc = (crc << 1) ^ 0x07;
      } else {
        crc <<= 1;
      }
    }
  }
  return crc;
}

void SensorPayload::add(uint8_t type, std::vector<uint8_t> value) {
  tlvs.push_back({type, static_cast<uint8_t>(value.size()), value});
}

std::vector<uint8_t> SensorPayload::encode() {
  // Encode the TLVs without packet length or CRC first
  std::vector<uint8_t> data = SensorTLVUtils::encode(tlvs);

  // Calculate total length: TLV data + PACKET_LENGTH_TAG TLV (3 bytes) + CRC (1
  // byte)
  size_t totalLength = data.size() + 3 + 1;

  if (totalLength > MAX_PAYLOAD_SIZE) {
    ESP_LOGE(TAG, "Payload exceeds max size of %d bytes: %d", MAX_PAYLOAD_SIZE,
             totalLength);
    return std::vector<uint8_t>(); // Return empty vector on overflow
  }

  // Create the final payload
  std::vector<uint8_t> encoded;
  encoded.reserve(totalLength);

  // Add PACKET_LENGTH_TAG (2 bytes in two’s complement)
  uint16_t lengthValue = static_cast<uint16_t>(totalLength);
  encoded.push_back(PACKET_LENGTH_TAG);
  encoded.push_back(2); // Length of value field
  encoded.push_back(static_cast<uint8_t>(lengthValue >> 8));   // MSB
  encoded.push_back(static_cast<uint8_t>(lengthValue & 0xFF)); // LSB

  // Append the rest of the TLVs
  encoded.insert(encoded.end(), data.begin(), data.end());

  // Calculate and append CRC
  uint8_t crc = calculateCRC8(encoded);
  encoded.push_back(crc);

  return encoded;
}
