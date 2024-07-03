#include "NetworkList.h"

NetworkList::NetworkList(): size(0){}
NetworkList::~NetworkList(){}

String* NetworkList::getSSID(uint8_t index){
    return &SSIDList[index];
}

String* NetworkList::getBSSIDstr(uint8_t index){
    return &BSSIDstrList[index];
}

uint8_t* NetworkList::getBSSID(uint8_t index){
    return BSSIDList[index];
}

float NetworkList::getRSSI(uint8_t index){
    return RSSIList[index];
}

uint8_t NetworkList::getChannel(uint8_t index){
    return channelList[size];
}

uint8_t NetworkList::getLength(){
    return size;
}

bool NetworkList::addNetwork(String& SSID, String& BSSIDstr, uint8_t* BSSID ,float RSSI, uint8_t channel){
    if(size == MAXSSIDS || !isNew(BSSID)){return false;}

    SSIDList[size] = SSID;
    BSSIDstrList[size] = BSSIDstr;
    memcpy(BSSIDList[size], BSSID, 6);
    RSSIList[size] = RSSI;
    channelList[size] = channel;

    size++;
    return true;
}

bool NetworkList::isNew(uint8_t* BSSID){
    for(uint8_t i=0; i< size; i++){
        if(memcmp(BSSIDList[i], BSSID, 6) == 0){
            return false;
        }
    }
    return true;
}


void NetworkList::printNetworks(){
    Serial.println("Found Networks: ");
    Serial.printf("     %-21s%-9s%-3s%s\n", "SSID", "MAC Address", "Channel", "RSSI");
    Serial.println("---------------------------------------------------------------");

    String offset;
    for(uint8_t i =0 ; i<size; i++){
        offset = i+1 >= 10 ? "  " : "   ";
        Serial.printf("#%d%s", i+1, offset);

        offset = "";
        for(uint8_t k; k < 25 - (*getSSID(i)).length(); k++){
            offset += " ";
        }

        Serial.printf("%s%s", *getSSID(i), offset);

        Serial.printf("%s%s", *getBSSIDstr(i), "   ");

        offset = getChannel(i) >= 10 ? "    " : "     ";
        Serial.printf("   %d%s", getChannel(i), offset);

        Serial.printf("%.2f", getRSSI(i));
        Serial.println("");
    }
}
