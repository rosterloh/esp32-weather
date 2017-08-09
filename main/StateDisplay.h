#ifndef MAIN_STATEDISPLAY_H_
#define MAIN_STATEDISPLAY_H_

#include <esp_types.h>

#define MODE_None				0
#define MODE_APNotConnected		1
#define MODE_STANotConnected	2
#define MODE_APConnected		3
#define MODE_STAConnected		4

class Wifi;

class StateDisplay {
public:
	StateDisplay();
	virtual ~StateDisplay();

	void SetAPMode(bool b) { mbAPMode = b; };
	void SetConnected(bool b) { mbConnected = b; };

	void Display();

private:
	bool mbAPMode;
	bool mbConnected;

	__uint8_t muState;
	__uint16_t muStateTimer;
};

#endif /* MAIN_STATEDISPLAY_H_ */