#ifndef NETWORKLIST_H
#define NETWORKLIST_H

#define MAXSSIDS 50

class NetworkList{
    public:
        NetworkList();
        ~NetworkList();
        String* getSSID(uint8_t);
        String* getBSSIDstr(uint8_t);
        uint8_t* getBSSID(uint8_t);
        float getRSN(uint8_t);
        uint8_t getLength();
        bool addNetwork(String&, String&, uint8_t*,  float);

    private:
        uint8_t size;
        String SSIDList[MAXSSIDS];
        String BSSIDstrList[MAXSSIDS];
        uint8_t* BSSIDList[MAXSSIDS];
        float RSNList[MAXSSIDS];
        void printNetworks();


};

#endif