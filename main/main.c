#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event_loop.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"

#define TAG "WIFI_STATUS"

#define SSID "my_network"
#define PASSWORD "wifipwd199777qp"

xSemaphoreHandle onConnectionHandle;

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
	switch (event->event_id)
	{
	case SYSTEM_EVENT_STA_START:
		esp_wifi_connect();
		ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START occurred, attempting to connect");
		xSemaphoreGive(onConnectionHandle);
		break;

	case WIFI_EVENT_STA_CONNECTED:
		ESP_LOGI(TAG, "WIFI_EVENT_STA_CONNECTED occurred");
		break;

	case SYSTEM_EVENT_STA_GOT_IP:
		ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP occurred");
		break;

	case WIFI_EVENT_STA_DISCONNECTED:
		ESP_LOGI(TAG, "WIFI_EVENT_STA_DISCONNECTED occurred");
		break;

	default:
		break;
	}
	return ESP_OK;
}

void wifi_init()
{
	ESP_ERROR_CHECK(nvs_flash_init());
	tcpip_adapter_init();
	ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

	wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

	wifi_config_t wifi_config = {
		.sta = {
			.ssid = SSID,
			.password = PASSWORD}};

	esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);

	ESP_ERROR_CHECK(esp_wifi_stop());
	vTaskDelay(pdMS_TO_TICKS(2000));
	ESP_ERROR_CHECK(esp_wifi_start());
}

void onConnected(void * params)
{
	while (true)
	{
		if(xSemaphoreTake(onConnectionHandle, pdMS_TO_TICKS(10UL * 1000UL)))
		{
			//do something
			ESP_LOGI(TAG, "Semaphore taken");

			xSemaphoreTake(onConnectionHandle, portMAX_DELAY);
		}
		else
		{
			ESP_LOGE("CONNECTION", "Unable to  connect, restarting.");
			esp_restart();
		}
		
	}
	
}

void app_main(void)
{
	ESP_LOGI(TAG, "-----------------------------------------------APP MAIN STARTED-----------------------------------------------");
	
	onConnectionHandle = xSemaphoreCreateBinary();
	wifi_init();

	xTaskCreate(&onConnected, "On Connected", 1024UL * 4UL, NULL, 5, NULL);

	while (true)
	{
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
