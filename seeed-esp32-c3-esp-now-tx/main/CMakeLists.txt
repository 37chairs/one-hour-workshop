idf_component_register(SRCS "main.cpp"
                    PRIV_REQUIRES "esp_driver_gpio" "esp_event" "esp_netif" "esp_http_client" "esp_http_server" "nvs_flash" "freertos" 
                    INCLUDE_DIRS ""
                    REQUIRES "sensor_tlv_utils" "esp_wifi")
