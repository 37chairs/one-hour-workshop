idf_component_register(SRCS "src/sensor_tlv_utils.cpp"
                            "src/sensor_node.cpp"
                            "src/sensor_payload.cpp"
                       INCLUDE_DIRS "include"
                       REQUIRES esp_wifi nvs_flash)

