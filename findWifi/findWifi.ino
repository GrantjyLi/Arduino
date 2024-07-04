#include <ESP8266WiFi.h>
#include "NetworkList.h"

NetworkList networks;

void setup() {
  Serial.begin(115200);
  Serial.flush();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); // Disconnect from any previously connected network
  
  delay(20); // Wait for the module to disconnect

  Serial.println("\nScanning for available networks:");

}

void loop(){
  findNewNetworks();
  networks.printNetworks();
  Serial.println("\n\n");
  delay(5000);
}

void findNewNetworks(){
  uint8_t newNumNetworks = WiFi.scanNetworks();
  String SSID;
  String BSSIDstr;
  uint8_t BSSID[6];
  float RSSI;
  uint8_t channel;

  for (uint8_t i = 0; i < newNumNetworks; i++){
    SSID = WiFi.SSID(i);
    BSSIDstr = WiFi.BSSIDstr(i);
    memcpy(BSSID, WiFi.BSSID(i), 6);
    RSSI = WiFi.RSSI(i);
    channel = WiFi.channel(i);

    // if(networks.addNetwork(SSID, BSSIDstr, BSSID, RSSI, channel)){
    //   Serial.println("-------------------------------------------");
    //   Serial.printf("SSID: %s\n", SSID.c_str());
    //   Serial.printf("BSSID: %s\n", BSSIDstr.c_str());
    // }

    networks.addNetwork(SSID, BSSIDstr, BSSID, RSSI, channel);
   
  }
  
}