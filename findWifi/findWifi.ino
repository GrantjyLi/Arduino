#include <ESP8266WiFi.h>

#define NUMNET 30

uint8_t numNetworks = 0;
String knownMACS[NUMNET];

void setup() {
  Serial.begin(9600);
  Serial.flush();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); // Disconnect from any previously connected network
  
  delay(1000); // Wait for the module to disconnect

  Serial.println("Scanning for available networks:");
  numNetworks = 0;
}

void loop(){
  findNewNetworks();
  delay(5000);
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
      //network name, mac address, signal strength
      Serial.println("--------------------------");
      Serial.printf("SSID: %s\n", WiFi.SSID(i).c_str());
      Serial.printf("MAC: %s\n", WiFi.BSSIDstr(i).c_str());
      Serial.printf("Channel: %d\n", WiFi.channel(i));
      Serial.printf("Strength: %f\n", WiFi.RSSI(i));

      knownMACS[numNetworks] = newMAC;
      numNetworks++;
    }
  }
}