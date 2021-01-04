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

#define AX_APs 20

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
	return ESP_OK;
}

void wifi_init()
{
	ESP_ERROR_CHECK(nvs_flash_init());
	tcpip_adapter_init();
	ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
}

void scan_wifi_aps(void *params)
{
	wifi_init();

	while (true)
	{
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void app_main(void)
{
	//uint32_t stack_size = uxTaskGetStackHighWaterMark(NULL);
	uint32_t stack_size = 32768;

	printf("Allocating %d bytes for main task\n", stack_size);

	//xTaskCreate(&heartbeat, "heartbeat", 2048, NULL, 1, NULL);
	//xTaskCreate(&scan_wifi_aps, "scan wifi aps", stack_size, NULL, 2, NULL);
}
