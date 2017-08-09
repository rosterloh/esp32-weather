#include <freertos/FreeRTOS.h>
#include "ESP32.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include <esp_log.h>

static const char TAG[] = "[ESP32]";

extern "C" {
	void app_main();
}

void task_function_display(void *pvParameter) {
	((ESP32*)pvParameter)->TaskDisplay();
	vTaskDelete(NULL);
}

ESP32::ESP32() {
	mWifi.SetConfig(&mConfig);
	mWifi.SetStateDisplay(&mStateDisplay);
}

ESP32::~ESP32() {}

void ESP32::Start() {
	ESP_LOGI(TAG, "Startup...");
    ESP_LOGI(TAG, "Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "IDF version: %s", esp_get_idf_version());
    ESP_LOGI(TAG, "Firmware version %s", FIRMWARE_VERSION);

	mConfig.Read();

	mbButtonPressed = !gpio_get_level(GPIO_NUM_0);

	gpio_pad_select_gpio(GPIO_NUM_0);
	gpio_set_direction(GPIO_NUM_0, GPIO_MODE_INPUT);
	gpio_set_pull_mode(GPIO_NUM_0, GPIO_PULLUP_ONLY);

	gpio_pad_select_gpio(GPIO_NUM_5);
	gpio_set_direction(GPIO_NUM_5, GPIO_MODE_OUTPUT);

	xTaskCreate(&task_function_display, "Task_Display", 2048, this, 5, NULL);

	if (mConfig.mbAPMode){
		if (mConfig.muLastSTAIpAddress){
			char sBuf[16];
			sprintf(sBuf, "%d.%d.%d.%d", IP2STR((ip4_addr*)&mConfig.muLastSTAIpAddress));
			ESP_LOGD(TAG, "Last IP when connected to AP: %d : %s", mConfig.muLastSTAIpAddress, sBuf);
		}
		mWifi.StartAPMode(mConfig.msAPSsid, mConfig.msAPPass, mConfig.msHostname);
	} else {
		mWifi.StartSTAMode(mConfig.msSTASsid, mConfig.msSTAPass, mConfig.msHostname);
	
		SetId();
		mMqtt.Init(this);
	}
}

void ESP32::TaskDisplay() {
	while (1) {
		mStateDisplay.Display();

		if (!gpio_get_level(GPIO_NUM_0)) {
			if (!mbButtonPressed) {
				ESP_LOGI(TAG, "button pressed");
				vTaskDelay(200);
				mConfig.ToggleAPMode();
				mConfig.Write();
				if (mConfig.mbAPMode) {
					ESP_LOGI(TAG, "enter AP mode");
				} else {
					ESP_LOGI(TAG, "enter standard mode");					
				}
				esp_restart();
			}
		} else
			mbButtonPressed = false;

		vTaskDelay(1);
	}
}

void ESP32::SetId() {
	char sHelp[20];
	mWifi.GetMac((__uint8_t*)sHelp);
	mId = "";
	mId.printf("esp32-%x%x%x%x%x%x", sHelp[0], sHelp[1], sHelp[2], sHelp[3], sHelp[4], sHelp[5]);
	mConfig.msDeviceId = mId;
	mConfig.Write();	
}

String& ESP32::GetId() {
	mId = mConfig.msDeviceId;
	return mId;
}

ESP32 esp32;

void app_main(){

	nvs_flash_init();
	tcpip_adapter_init();

	esp32.Start();
}