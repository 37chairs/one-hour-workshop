#ifndef SENSOR_TLV_UTILS_H
#define SENSOR_TLV_UTILS_H

#include <vector>
#include <cstdint>

class SensorTLVUtils {
public:
    struct TLV {
        uint8_t type;
        uint8_t length;
        std::vector<uint8_t> value;
    };

    static std::vector<uint8_t> encode(const std::vector<TLV>& tlvs);
    static std::vector<TLV> decode(const std::vector<uint8_t>& data);
};

#endif // SENSOR_TLV_UTILS_H

