#ifndef MAIN_MQTT_H_
#define MAIN_MQTT_H_

#include <aws_iot_mqtt_client.h>
#include "Config.h"
#include "String.h"

class ESP32;

class MQTT {
public:
    MQTT();
	virtual ~MQTT();
    
    bool Init(ESP32* pEsp);
    void ProcessConfigChange();
    bool Connect();
    void Shutdown();
    bool Run();
    bool Publish(const char* pTopic, short pTopicLenth, String* pPayload);

    bool mInitialized = false;
    bool mConnected = false;
    bool mActive = false;

private:
    ESP32* mpEsp;  
    Config* mpConfig;

    AWS_IoT_Client client;
	IoT_Client_Init_Params mqttInitParams;
	IoT_Client_Connect_Params connectParams;
};

#endif /* MAIN_MQTT_H_ */