
#include <ESP8266WiFi.h>
#include "defs.h"

using namespace defs;

extern "C" {
  #include "user_interface.h"
}
int arrayindex =0;

void setup() {
  delay(500);
  wifi_set_opmode(STATION_MODE);
  wifi_promiscuous_enable(1); 
  Serial.begin(115200);
  Serial.println("");
  arrayindex =0;
}

void sendBeacon(const char* ssid){
    int ssidSize = strlen(ssid);
    int packetSize = 38 + ssidSize + sizeof(postSSID);

    packet[37] = ssidSize;

    //copying SSID into packet an post SSID
    memcpy(&packet[38], ssid, ssidSize);
    memcpy(&packet[38 + ssidSize], postSSID, sizeof(postSSID));

    for(int k=0; k< 6; k++){
        packet[10 + k] = packet[16 + k] = random(256);
    }
    
    //looping through every wifi channel
    for(int i=0; i < 3 ; i++){
        // Randomize SRC MAC
    

        wifi_set_channel(channels[i]);
        packet[50 + ssidSize] = channels[i];

        for(int i=0; i<3; i++){
            wifi_send_pkt_freedom(packet, packetSize, 0);
            delay(1);
        }
        
    }
    
}

void loop() {
    
    sendBeacon(ssids[arrayindex]);
    arrayindex++;
    if(arrayindex >= 11){arrayindex =0;}
}