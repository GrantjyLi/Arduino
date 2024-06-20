#include <ESP8266WiFi.h>
#include "defs.h"

using namespace defs;

bool wpa2 = false;
char emptySSID[32];
uint8_t macAdd[6];
uint8_t wifiChannel;
uint32_t packetSize;

char ssid[] = "cheese\n";

void setup(){
    packetSize = sizeof(beaconPacket);
    initSSID(emptySSID);

    Serial.begin(115200);
    Serial.println();
    
    WiFi.mode(WIFI_OFF);
    wifi_set_opmode(STATION_MODE);

    if (wpa2) {
        beaconPacket[34] = 0x31;
    } else {
        beaconPacket[34] = 0x21;
        packetSize -= 26;
    }
}

void loop(){

    for(int i=0; i<NUM_BEACONS; i++){
        randomMac(macAdd);

        //copy fake mac address into the packet
        memcpy(&beaconPacket[10], macAdd, 6);
        memcpy(&beaconPacket[16], macAdd, 6);

        //reset and write ssid into the packet
        memcpy(&beaconPacket[38], emptySSID, 32);
        memcpy_P(&beaconPacket[38], ssid, sizeof(ssid));

        //switch wifi channels
        for(int i=0; i< sizeof(channels); i++){
            wifiChannel = channels[i];
            beaconPacket[82] = wifiChannel;
            wifi_set_channel(wifiChannel);

            wifi_send_pkt_freedom(beaconPacket, packetSize, 0);
            delay(1);
        }
    }

    
}

void randomMac(uint8_t * macAdd){
    for(int i=0; i< 6; i++){
        macAdd[i] = random(256);
    }
}

void initSSID(char* SSID){
    for(int i=0; i< 32; i++){
        SSID[i] = ' ';
    }
}