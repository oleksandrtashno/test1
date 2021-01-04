#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "esp_sntp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "protocol_examples_common.h"

#define TAG	"NTP_TIME"

void on_time_received(struct timeval *tv)
{
	setenv("TZ", "EET-2EEST,M3.5.0/3,M10.5.0/4", 1);
	tzset();
	struct tm *timeinfo = localtime(&tv->tv_sec);

	char buffer[64];

	strftime(buffer, sizeof(buffer), "%c", timeinfo);
	ESP_LOGI(TAG, "Time now is %s", buffer);

	example_disconnect();
}

void app_main(void)
{
	nvs_flash_init();
	tcpip_adapter_init();
	esp_event_loop_create_default();
	example_connect();

	sntp_set_sync_mode(SNTP_SYNC_MODE_IMMED);
	sntp_setservername(0, "pool.ntp.org");
	sntp_init();
	sntp_set_time_sync_notification_cb(on_time_received);
}
