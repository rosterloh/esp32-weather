# [ESP32 Weather](https://github.com/rosterloh/esp32-weather)

### Setup

Install [ESP IoT Development Framework](https://github.com/espressif/esp-idf) and [configure](http://esp-idf.readthedocs.io/en/latest/get-started/index.html) 

### Set WiFi settings

Run
    
    make menuconfig
    
and navigate to `Wifi Configuration`. Enter the ssid and password here.

### Development

Build and debug with 

    make flash monitor

## SSL Keys

Device certificates and keys were generated using the following options

    openssl genrsa -out client.key 2048
    openssl req -new -out client.csr -key client.key