#include <ESP8266WiFi.h>        // Include the Wi-Fi library

const char* ssid = "GPhone";
const char* password = "hotspot4Grant";

void setup() {
   // Start serial
  Serial.begin(115200);
  delay(100);

  // Connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  
}
