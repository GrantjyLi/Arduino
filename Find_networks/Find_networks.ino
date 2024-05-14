#include <ESP8266WiFi.h>

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); // Disconnect from any previously connected network
  
  delay(1000); // Wait for the module to disconnect

  Serial.println("Scanning for available networks:");
  int numNetworks = WiFi.scanNetworks();
  
  if (numNetworks == 0) {
    Serial.println("No networks found.");
  } else {
    Serial.print(numNetworks);
    Serial.println(" networks found:");
    for (int i = 0; i < numNetworks; ++i) {
      Serial.println(WiFi.SSID(i));
    }
  }
}

void loop() {
  
}
