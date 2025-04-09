#ifndef NETWORKLIST_H
#define NETWORKLIST_H

#include <Arduino.h>
#include "Network.h"

#define MAXSSIDS 50

class NetworkList{
    public:
        NetworkList();
        ~NetworkList();
        int8_t getSize();
        bool addNetwork(String&, String&, uint8_t*,  float, uint8_t);
        bool isNew(uint8_t*);
        Network* getNetwork(uint8_t index);
        void printNetworks();

    private:
        int8_t size;
        Network* networks[MAXSSIDS];
};

#endif