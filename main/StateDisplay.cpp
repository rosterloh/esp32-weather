#include "StateDisplay.h"
#include "driver/gpio.h"

StateDisplay::StateDisplay() {
	mbAPMode = false;
	mbConnected = false;
	muState = 0;
	muStateTimer = 0;
}

StateDisplay::~StateDisplay() {}

void StateDisplay::Display() {
	//ESP_LOGD("StateDisplay", "Display %d, %d, %d, %d\n", mbAPMode, mbConnected, muState, muStateTimer);
	if (mbAPMode) {
		if (mbConnected) {
			switch (muState) {
				case 0:
					if (muStateTimer)
						muStateTimer--;
					else {
						muStateTimer = 200;
						muState = 1;
						gpio_set_level((gpio_num_t) GPIO_NUM_5, 25);
					}
					break;
				case 1:
					if (muStateTimer)
						muStateTimer--;
					else {
						muStateTimer = 200;
						muState = 0;
						gpio_set_level((gpio_num_t) GPIO_NUM_5, 0);
					}
					break;
				default:
					muStateTimer = 0;
					muState = MODE_None;
			}
		} else {
			switch (muState) {
				case 0:
					if (muStateTimer)
						muStateTimer--;
					else {
						muStateTimer = 200;
						muState = 1;
						gpio_set_level((gpio_num_t) GPIO_NUM_5, 25);
					}
					break;
				case 1:
					if (muStateTimer)
						muStateTimer--;
					else {
						muStateTimer = 200;
						muState = 0;
						gpio_set_level((gpio_num_t) GPIO_NUM_5, 0);
					}
					break;
				default:
					muStateTimer = 0;
					muState = 0;
			}
		}
	} else {
		if (mbConnected) {
			gpio_set_level((gpio_num_t) GPIO_NUM_5, 75);
		} else {
			switch (muState) {
				case 0:
					if (muStateTimer)
						muStateTimer--;
					else {
						muStateTimer = 40;
						muState = 1;
						gpio_set_level((gpio_num_t) GPIO_NUM_5, 25);
					}
					break;
				case 1:
					if (muStateTimer)
						muStateTimer--;
					else {
						muStateTimer = 200;
						muState = 0;
						gpio_set_level((gpio_num_t) GPIO_NUM_5, 0);
					}
					break;
				default:
					muStateTimer = 0;
					muState = 0;
			}
		}
	}
}