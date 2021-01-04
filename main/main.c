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

static char *getAuthModeName(wifi_auth_mode_t auth_mode)
{
  char *names[] = {"OPEN", "WEP", "WPA PSK", "WPA2 PSK", "WPA WPA2 PSK", "MAX"};
  return names[auth_mode];
}

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
	return ESP_OK;
}

void wifi_init()
{
	ESP_ERROR_CHECK(nvs_flash_init());
	tcpip_adapter_init();
	ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

	wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
	ESP_ERROR_CHECK(esp_wifi_start());
}

void scan_wifi_aps(void *params)
{
	wifi_init();

	wifi_scan_config_t scan_config = {
		.ssid = 0,
		.bssid = 0,
		.channel = 0,
		.show_hidden = true,
		.scan_time = {.active = {.min = 1000, .max = 1300}},
		.scan_type = WIFI_SCAN_TYPE_ACTIVE
	};

	ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, true));

	wifi_ap_record_t scan_results[MAX_APs];

	uint16_t ap_count = MAX_APs;

	ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&ap_count, scan_results));

	printf("Found %d access points:\n", ap_count);
	printf("\n");
	printf("               SSID              | Channel | RSSI |   Auth Mode \n");
	printf("----------------------------------------------------------------\n");
	for (int i = 0; i < ap_count; i++)
		printf("%32s | %7d | %4d | %12s\n", (char *)scan_results[i].ssid, scan_results[i].primary, scan_results[i].rssi, getAuthModeName(scan_results[i].authmode));
	printf("----------------------------------------------------------------\n");

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
	xTaskCreate(&scan_wifi_aps, "scan wifi aps", stack_size, NULL, 2, NULL);
}
