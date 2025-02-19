// sensor_tags.h
#ifndef SENSOR_TAGS_H
#define SENSOR_TAGS_H

#define PACKET_LENGTH_TAG                                                      \
  0x01                   // Total length of payload (including this TLV and CRC)
#define ID_TAG 0x10      // Node ID
#define COUNTER_TAG 0x11 // Packet counter
#define VERSION_TAG 0x12 // Firmware version
#define VOLTAGE_TAG 0x13 // Battery voltage

#define TEMPERATURE_TAG 0x20   // Temperature in °F (x10 for precision)
#define HUMIDITY_TAG 0x21      // Relative humidity (% x10)
#define SOIL_MOISTURE_TAG 0x22 // Soil moisture (% x10)
#define LUMINOSITY_TAG 0x23    // Light intensity (lux x10)
#define SWITCH_STATE_TAG 0x30  // Binary switch state (0 or 1)

#endif // SENSOR_TAGS_H
