#include "Network.h"

Network::Network(String& SSID, String& BSSIDstr, uint8_t* BSSID, float RSSI, uint8_t channel){
    this->SSID = SSID;
    this->BSSIDstr = BSSIDstr;
    memcpy(this->BSSID, BSSID, MAC_ADDR_LEN);
    this->RSSI = RSSI;
    this->channel = channel;
}
   
Network::~Network(){}