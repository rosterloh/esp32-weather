# [ESP32 Weather](https://github.com/rosterloh/esp32-weather)

### Setup

Install [ESP-IDF](https://github.com/espressif/esp-idf) and [configure](http://esp-idf.readthedocs.io/en/latest/get-started/index.html) 

### Set WiFi settings

Run
    
    make menuconfig
    
and navigate to `Local Configuration`. Enter the ssid and password here.

### Development

Build and debug with 

    make flash monitor