# T-Dongle-S3 LCD Counter Example (ESP-IDF)

This project demonstrates how to interface an ST7735S-based display with the **LilyGO T-Dongle-S3** using **ESP-IDF**. It initializes the display, configures the SPI bus, and continuously updates a counter on the screen.

## Project Overview

- Uses **ESP-IDF** (not Arduino/PlatformIO).
- Configures **SPI communication** manually.
- Uses **LovyanGFX** for display handling.
- Implements **FreeRTOS tasks** for periodic updates.

## Hardware Configuration

This project is designed for the **T-Dongle-S3**, which has a built-in **ST7735S** display. The following GPIOs are used:

| Signal | GPIO |
| --- | --- |
| RST (Reset) | 1   |
| DC (Data/Command) | 2   |
| MOSI (SPI Data Out) | 3   |
| CS (Chip Select) | 4   |
| SCLK (SPI Clock) | 5   |
| LEDA (Backlight) | 38  |
| MISO (Not Used) | \-1 |
| BUSY (Not Used) | \-1 |

## SPI Bus Configuration

The display is connected via **SPI**, manually configured as follows:

- **Clock Speed:** 27 MHz (write), 16 MHz (read)
- **Mode:** SPI Mode 0
- **Bus Sharing:** Enabled

## Backlight Control

The display backlight is controlled via **GPIO 38**. It is **inverted** and uses **PWM channel 7** for brightness control.

```cpp
cfg.pin_bl = DISPLAY_LEDA; 
cfg.invert = true; 
cfg.freq = 12000; 
cfg.pwm_channel = 7;
```

## FreeRTOS Task for Updating the Display

A **FreeRTOS task** (`taskCounter`) updates the screen every second with an incrementing counter.

```cpp
void taskCounter(void *pvParameters) {
    int counter = 0;
    while (true) {
        tft.clear();
        tft.setCursor(0, 0);
        tft.printf("Counter: \n> %d", counter++);
        tft.display();
        if (counter > 1000) {
            counter = 0;
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

## Build and Flash Instructions

### 1\. Set Up ESP-IDF

Ensure you have ESP-IDF installed and sourced:

```sh
source ~/esp/esp-idf/export.sh
```

### 2\. Configure and Build

Navigate to the project directory and build:


```sh
idf.py set-target esp32s3
idf.py menuconfig
idf.py build
```

### 3\. Flash to the Board

```sh
idf.py flash
```

