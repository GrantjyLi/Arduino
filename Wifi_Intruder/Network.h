#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>

#define MAC_ADDR_LEN 6

class Network{
    public:
        Network(String&, String&, uint8_t*, float, uint8_t);
        ~Network();

        String SSID;
        String BSSIDstr;
        uint8_t BSSID[MAC_ADDR_LEN];
        float RSSI;
        uint8_t channel;       

};

#endif