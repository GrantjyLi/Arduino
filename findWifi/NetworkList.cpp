#include "NetworkList.h"

NetworkList::NetworkList(): size(0){
    
}

String* NetworkList::getSSID(uint8_t index){
    return &SSIDList[index];
}

String* NetworkList::getBSSIDstr(uint8_t index){
    return &BSSIDstrList[index];
}

uint8_t* NetworkList::getBSSID(uint8_t index){
    return BSSIDList[index];
}

float NetworkList::getRSN(uint8_t index){
    return RSNList[index];
}

uint8_t NetworkList::getLength(){
    return size;
}

bool NetworkList::addNetwork(String& SSID, String& BSSIDstr, uint8_t* BSSID ,float RSN){
    if(size == MAXSSIDS){return false;}

    SSIDList[size] = SSID;
    BSSIDstrList[size] = BSSIDstr;
    memcpy(BSSIDList[size], BSSID, 6);
    RSNList[size] = RSN;

    size++;
    return true;
}

