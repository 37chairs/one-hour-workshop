idf_component_register(SRCS "main.cpp"
                    PRIV_REQUIRES spi_flash
                    INCLUDE_DIRS ""
                    REQUIRES "led_strip_spi"
                    )

