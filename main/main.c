#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_http_client.h"
#include "protocol_examples_common.h"
#include "freertos/task.h"

#define MAX_APs 20

void app_main(void)
{
	//uint32_t stack_size = uxTaskGetStackHighWaterMark(NULL);
	uint32_t stack_size = 32768;

	printf("Allocating %d bytes for main task\n", stack_size);

	//xTaskCreate(&heartbeat, "heartbeat", 2048, NULL, 1, NULL);
	xTaskCreate(&scan_wifi_aps, "scan wifi aps", stack_size, NULL, 2, NULL);
}
