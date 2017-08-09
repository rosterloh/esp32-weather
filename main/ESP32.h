#ifndef MAIN_ESP32_H_
#define MAIN_ESP32_H_

#include "StateDisplay.h"
#include "MQTT.h"
#include "Wifi.h"
#include "Config.h"

#define FIRMWARE_VERSION __DATE__ " " __TIME__

class ESP32 {
public:
    ESP32();
	virtual ~ESP32();

    void Start();

	void TaskDisplay();

    Config& 				GetConfig()	{ return mConfig; };
	Wifi& 					GetWifi()	{ return mWifi; };
    MQTT&			        GetMqtt()   { return mMqtt; };
    String&					GetId();

private:
    void SetId();
    String mId;

    StateDisplay mStateDisplay;
    Wifi mWifi;
    Config mConfig;
    MQTT mMqtt;

    bool mbButtonPressed;
};

#endif