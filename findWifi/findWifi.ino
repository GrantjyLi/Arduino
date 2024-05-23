#include <ESP8266WiFi.h>

#define NUMSSID 30

uint8_t numNetworks = 0;
String knownSSIDs[NUMSSID];

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); // Disconnect from any previously connected network
  
  delay(1000); // Wait for the module to disconnect

  Serial.println("Scanning for available networks:");
  numNetworks = WiFi.scanNetworks();
  
  if (numNetworks == 0) {
    Serial.println("No networks found.");
  } else {
    Serial.print(numNetworks);
    Serial.println(" networks found:");
    for (int i = 0; i < numNetworks; i++) {
      knownSSIDs[i] = WiFi.SSID(i);
      Serial.println("--------------------------");
      Serial.print("SSID: ");
      Serial.println(WiFi.SSID(i));
      Serial.print("Strength: ");
      Serial.println(WiFi.RSSI(i));
    }
  }
}

void loop(){
  delay(5000);

  uint8_t newNumNetworks = WiFi.scanNetworks();

  for (uint8_t i = 0; i < newNumNetworks; i++){
    String newSSID = WiFi.SSID(i);
    bool newNetwork = true;

    for (uint8_t k = 0; k < numNetworks; k++){
      if(newSSID == knownSSIDs[k]){
        newNetwork = false;
        break;
      }
    }

    if(newNetwork){
      Serial.println("--------------------------");
        Serial.print("SSID: ");
        Serial.println(newSSID);
        Serial.print("Strength: ");
        Serial.println(WiFi.RSSI(i));

        knownSSIDs[numNetworks] = newSSID;
        numNetworks++;
    }
  }
}