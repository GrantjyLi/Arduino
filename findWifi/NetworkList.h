#ifndef NETWORKLIST_H
#define NETWORKLIST_H

#include <Arduino.h>

#define MAXSSIDS 50

class NetworkList{
    public:
        NetworkList();
        ~NetworkList();
        String* getSSID(uint8_t);
        String* getBSSIDstr(uint8_t);
        uint8_t* getBSSID(uint8_t);
        float getRSSI(uint8_t);
        uint8_t getChannel(uint8_t);
        uint8_t getLength();
        bool addNetwork(String&, String&, uint8_t*,  float, uint8_t);
        bool isNew(uint8_t*);
        void printNetworks();

    private:
        uint8_t size;
        String SSIDList[MAXSSIDS];
        String BSSIDstrList[MAXSSIDS];
        uint8_t BSSIDList[MAXSSIDS][6];
        float RSSIList[MAXSSIDS];
        uint8_t channelList[MAXSSIDS];


};

#endif