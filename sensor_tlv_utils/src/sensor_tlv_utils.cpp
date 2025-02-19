#include "sensor_tlv_utils.h"

std::vector<uint8_t> SensorTLVUtils::encode(const std::vector<TLV>& tlvs) {
    std::vector<uint8_t> encoded;
    for (const auto& tlv : tlvs) {
        encoded.push_back(tlv.type);
        encoded.push_back(tlv.length);
        encoded.insert(encoded.end(), tlv.value.begin(), tlv.value.end());
    }
    return encoded;
}

std::vector<SensorTLVUtils::TLV> SensorTLVUtils::decode(const std::vector<uint8_t>& data) {
    std::vector<TLV> decoded;
    size_t i = 0;
    while (i < data.size()) {
        if (i + 2 > data.size()) break;
        TLV tlv;
        tlv.type = data[i++];
        tlv.length = data[i++];
        if (i + tlv.length > data.size()) break;
        tlv.value.insert(tlv.value.end(), data.begin() + i, data.begin() + i + tlv.length);
        i += tlv.length;
        decoded.push_back(tlv);
    }
    return decoded;
}

