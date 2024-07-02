#ifndef NETWORKLIST_H
#define NETWORKLIST_H

#define MAXSSIDS 50

class NetworkList{
    public:
        String* getSSID(uint8_t);
        String* getBSSIDstr(uint8_t);
        float getRSN(uint8_t);
        uint8_t getLength();
        void addNetwork(String&, String&, float);
        ~NetworkList();

    private:
        uint8_t size;
        String ssidList[MAXSSIDS];
        String ssidList[MAXSSIDS];
        String ssidList[MAXSSIDS];


};

#endif