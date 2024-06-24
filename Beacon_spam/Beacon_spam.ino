
#include <ESP8266WiFi.h>
#include "defs.h"

using namespace defs;

extern "C" {
  #include "user_interface.h"
}
int arrayindex;
int packetsSent;
unsigned int lastTime;

void setup() {
    delay(500);
    wifi_set_opmode(STATION_MODE);
    wifi_promiscuous_enable(1); 

    arrayindex = 0;
    packetsSent = 0;
    lastTime = millis();

    Serial.begin(115200);
    Serial.println("\n");
}

void sendBeacon(const char* ssid){
    int ssidSize = strlen(ssid);
    int packetSize = 38 + ssidSize + sizeof(postSSID);

    beaconPacket[37] = ssidSize;

    //copying SSID into packet an post SSID
    memcpy(&beaconPacket[38], ssid, ssidSize);
    memcpy(&beaconPacket[38 + ssidSize], postSSID, sizeof(postSSID));

    // Randomize SRC MAC
    for(int k=0; k< 6; k++){
        beaconPacket[10 + k] = beaconPacket[16 + k] = random(256);
    }

    //looping through every wifi channel
    for(int i=0; i < NUM_CHANNELS ; i++){
        wifi_set_channel(channels[i]);
        beaconPacket[50 + ssidSize] = channels[i];

        //send it out 3 times to be sure
        for(int i=0; i<3; i++){
            packetsSent += wifi_send_pkt_freedom(beaconPacket, packetSize, 0) == 0;  
            delay(1);
        }
        
    }
    
}

void loop() {
    sendBeacon(ssids[arrayindex]);
    arrayindex++;
    if(arrayindex >= NUM_SSIDS){arrayindex =0;}

    if (millis() - lastTime > 2000){
        Serial.printf("Packets Sent: %d\n", packetsSent);
        lastTime = millis();
    }
}