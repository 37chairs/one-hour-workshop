#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

void hello_world_task(void *pvParameter) {
  while (true) {
    printf("Hello world!\n");
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

extern "C" void app_main(void) {
  printf("Starting app_main\n");
  vTaskDelay(pdMS_TO_TICKS(3000));

  xTaskCreate(hello_world_task, "hello_world_task", 4096, NULL, 5,
              NULL); // ✅ Increased stack size
}

