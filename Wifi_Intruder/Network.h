#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>

#define MAC_ADDR_LEN 6

class Network{
    public:
        Network(String&, String&, uint8_t*, float, uint8_t);
        ~Network();
        String* getSSID(){return &SSID;}
        String* getBSSIDstr(){return &BSSIDstr;}
        uint8_t* getBSSID(){return BSSID;}
        float getRSSI(){return RSSI;}
        uint8_t getChannel(){return channel;}

    private:
        String SSID;
        String BSSIDstr;
        uint8_t BSSID[MAC_ADDR_LEN];
        float RSSI;
        uint8_t channel;

};

    



#endif