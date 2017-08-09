#include "ESP32.h"
#include <aws_iot_config.h>
#include "mqtt_certs.h"
#include <aws_iot_mqtt_client_interface.h>
#include "Config.h"
#include "String.h"
#include <esp_log.h>
#include <cJSON.h>
#include "MQTT.h"
#include "temperature.h"

static const char* TAG = "[MQTT]";

void task_function_mqtt(void *pvParameter)
{
	((MQTT*)pvParameter)->Connect();
	((MQTT*)pvParameter)->Shutdown();
	vTaskDelete(NULL);
}

void iot_subscribe_callback_handler(AWS_IoT_Client *pClient, char *topicName, uint16_t topicNameLen,
									IoT_Publish_Message_Params *params, void *pData) {
	IOT_UNUSED(pData);
	IOT_UNUSED(pClient);
	ESP_LOGI(TAG, "Message received");
	ESP_LOGD(TAG, "%s: %s", topicName, (char*)params->payload);
}

void disconnectCallbackHandler(AWS_IoT_Client *pClient, void *data) {
	ESP_LOGW(TAG, "MQTT Disconnect");
	IoT_Error_t rc = FAILURE;

	if(NULL == pClient) {
		return;
	}

	IOT_UNUSED(data);

	if(aws_iot_is_autoreconnect_enabled(pClient)) {
		ESP_LOGI(TAG, "Auto Reconnect is enabled, Reconnecting attempt will start now");
	} else {
		ESP_LOGW(TAG, "Auto Reconnect not enabled. Starting manual reconnect...");
		rc = aws_iot_mqtt_attempt_reconnect(pClient);
		if(NETWORK_RECONNECTED == rc) {
			ESP_LOGW(TAG, "Manual Reconnect Successful");
		} else {
			ESP_LOGW(TAG, "Manual Reconnect Failed - %d", rc);
		}
	}
}

MQTT::MQTT() {}

MQTT::~MQTT() {}

bool MQTT::Init(ESP32* pEsp) {
    mpEsp = pEsp;  
    mpConfig = &(mpEsp->GetConfig());
	mInitialized = true;
    ProcessConfigChange();
	return mInitialized;
}

void MQTT::ProcessConfigChange(){
    if (!mInitialized)
        return; 

    ESP_LOGI(TAG, "Starting MQTT Thread");
	xTaskCreate(&task_function_mqtt, "Task_MQTT", 8192, this, 5, NULL);
}

bool MQTT::Connect() {

	vTaskDelay(12000 / portTICK_PERIOD_MS);
	ESP_LOGI(TAG, "Connecting");

    while (!mConnected) {
        if (mpEsp->GetWifi().IsConnected()) {
			ESP_LOGI(TAG, "Init");
			char HostAddress[255] = CONFIG_AWS_IOT_MQTT_HOST;	
			mqttInitParams = iotClientInitParamsDefault;

			mqttInitParams.enableAutoReconnect = false; // We enable this later below
			mqttInitParams.pHostURL = HostAddress;
			mqttInitParams.port = CONFIG_AWS_IOT_MQTT_PORT;
			mqttInitParams.pRootCALocation = rootCA;
			mqttInitParams.pDeviceCertLocation = deviceCert;
			mqttInitParams.pDevicePrivateKeyLocation = devicePrivateKey;
			mqttInitParams.mqttCommandTimeout_ms = 20000;
			mqttInitParams.tlsHandshakeTimeout_ms = 5000;
			mqttInitParams.isSSLHostnameVerify = true;
			mqttInitParams.disconnectHandler = disconnectCallbackHandler;
			mqttInitParams.disconnectHandlerData = NULL;

			IoT_Error_t rc = aws_iot_mqtt_init(&client, &mqttInitParams);
			if (rc) {
				ESP_LOGE(TAG, "Init Error: %i", rc);
				return false;
			}
			mConnected = true;
		}
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}

	connectParams = iotClientConnectParamsDefault;    
	connectParams.keepAliveIntervalInSec = 10;
	connectParams.isCleanSession = true;
	connectParams.MQTTVersion = MQTT_3_1_1;
	connectParams.pClientID = mpEsp->GetId().c_str();
	connectParams.clientIDLen = mpEsp->GetId().length();
	connectParams.isWillMsgPresent = false;

	ESP_LOGI(TAG, "Connecting to %s", AWS_IOT_MQTT_HOST);
    IoT_Error_t rc = aws_iot_mqtt_connect(&client, &connectParams);
    if (rc) {
        ESP_LOGE(TAG, "Connect Error: %i", rc);
		return false;
    }
	ESP_LOGI(TAG, "connected successfully");
    return Run();
}

bool MQTT::Run() {
	IoT_Error_t rc;
	
	rc = aws_iot_mqtt_autoreconnect_set_status(&client, true);
	if(SUCCESS != rc) {
		ESP_LOGE(TAG, "Unable to set Auto Reconnect to true - %d", rc);
		return false;
	}
/*
	String topic("/weather/");
	topic.printf("%s", AWS_IOT_MQTT_CLIENT_ID);
	ESP_LOGI(TAG, "Subscribing to %s", topic.c_str());
	rc = aws_iot_mqtt_subscribe(&client, topic.c_str(), topic.length(), QOS1, iot_subscribe_callback_handler, NULL);
	if(SUCCESS != rc) {
		ESP_LOGE(TAG, "Error subscribing : %d ", rc);
		return false;
	}
	ESP_LOGI(TAG, "subscription successful");
*/
	mActive = true;
	while (mActive) {
        float t = (float) temprature_sens_read() * (125.0 + 40.0) / 255.0 - 40.0;
        //ESP_LOGI(TAG, "Temperature %3.1f", t);
		String payload(t, 2);
		Publish("/weather/temperature", 20, &payload);
		vTaskDelay(10000 / portTICK_PERIOD_MS);
	}
	
	return mActive;
}

bool MQTT::Publish(const char* pTopic, short pTopicLength, String* pPayload) {

	IoT_Publish_Message_Params params;
	IoT_Error_t rc = FAILURE;
	
	params.qos = QOS0;
	params.payload = (void *) pPayload->c_str();
	params.payloadLen = pPayload->length();
	params.isRetained = 0;

	//Max time the yield function will wait for read messages
	rc = aws_iot_mqtt_yield(&client, 100);
	while (NETWORK_ATTEMPTING_RECONNECT == rc) {
		rc = aws_iot_mqtt_yield(&client, 100);
	}

	params.payloadLen = pPayload->length();

	rc = aws_iot_mqtt_publish(&client, pTopic, pTopicLength, &params);

	if (rc == MQTT_REQUEST_TIMEOUT_ERROR) {
		ESP_LOGW(TAG, "QOS1 publish ack not received.");
		rc = SUCCESS;
	}

	if(SUCCESS != rc) {
		ESP_LOGE(TAG, "An error occurred in Publish: %i", rc);
	} else {
		ESP_LOGI(TAG, "successfully published to %s", pTopic);
	}

	return rc;	
}

void MQTT::Shutdown() {
	ESP_LOGI(TAG, "Shutdown");
	mConnected = false;
	mActive = false;
}