#include "NetworkList.h"

NetworkList::NetworkList(): size(0){}
NetworkList::~NetworkList(){
    for(int8_t i = 0; i< size; i++){delete networks[i];}
}

int8_t NetworkList::getSize(){
    return size;
}

bool NetworkList::addNetwork(String& SSID, String& BSSIDstr, uint8_t* BSSID ,float RSSI, uint8_t channel){
    if(size == MAXSSIDS || !isNew(BSSID)){return false;}
    networks[size] = new Network(SSID, BSSIDstr, BSSID, RSSI, channel);
    size++;
    return true;
}

bool NetworkList::isNew(uint8_t* BSSID){
    for(uint8_t i=0; i< size; i++){
        if(memcmp(networks[i]->getBSSID(), BSSID, MAC_ADDR_LEN) == 0){
            return false;
        }
    }
    return true;
}

Network* NetworkList::getNetwork(uint8_t index){
    if(index < size){
        return networks[index];
    }else{
        return nullptr;
    }
}

void NetworkList::printNetworks(){
    Serial.println("Found Networks: ");
    Serial.printf("#    %-34s%-20s%-10s%s\n", "SSID", "MAC Address", "Channel", "RSSI");
    Serial.println("-------------------------------------------------------------------------");

    String offset;
    for(uint8_t i =0 ; i<size; i++){
        Network* network = networks[i];
        String* SSID = network->getSSID();
        String* BSSIDstr = network->getBSSIDstr();
        uint8_t* BSSID = network->getBSSID();
        float RSSI = network->getRSSI();
        uint8_t channel = network->getChannel();

        offset = i+1 >= 10 ? "  " : "   ";
        Serial.printf("#%d%s", i+1, offset.c_str());

        Serial.printf("%s", SSID->c_str());

        for(uint8_t k =0 ; k < 34 - SSID->length(); k++){
            Serial.print(" ");
        }

        Serial.printf("%s%s", BSSIDstr->c_str(), "   ");

        offset = channel >= 10 ? "    " : "     ";
        Serial.printf("   %d%s", channel, offset.c_str());

        Serial.printf("%.2f", RSSI);
        Serial.println("");
    }
}