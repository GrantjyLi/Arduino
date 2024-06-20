#include <ESP8266WiFi.h>
#include "defs.h"

using namespace defs;

extern "C" {
    #include "user_interface.h"
    int wifi_send_pkt_freedom(uint8 *buf, int len, bool sys_seq);
}

const bool wpa2 = false;
char emptySSID[32];
uint8_t macAdd[6];
uint8_t wifiChannel;
uint32_t packetSize;

char ssid[] = "cheese\n";

void setup(){
    packetSize = sizeof(beaconPacket);
    initSSID();

    Serial.begin(115200);
    Serial.println();
    
    WiFi.mode(WIFI_OFF);
    wifi_set_opmode(STATION_MODE);
    wifi_promiscuous_enable(0);

    if (wpa2) {
        beaconPacket[34] = 0x31;
    } else {
        beaconPacket[34] = 0x21;
        packetSize -= 26;
    }
}

void loop(){

    for(int i=0; i<NUM_BEACONS; i++){
        randomMac();

        //copy fake mac address into the packet
        memcpy(&beaconPacket[10], macAdd, 6);
        memcpy(&beaconPacket[16], macAdd, 6);

        //reset and write ssid into the packet
        memcpy(&beaconPacket[38], emptySSID, 32);
        memcpy_P(&beaconPacket[38], ssid, sizeof(ssid) -1);

        //switch wifi channels
        for(int i=0; i< sizeof(channels); i++){
            wifiChannel = channels[i];
            beaconPacket[82] = wifiChannel;
            wifi_set_channel(wifiChannel);

            if(wifi_send_pkt_freedom(beaconPacket, packetSize, 0) != 0 ){
                Serial.println("Packet Failed to send.");
            }else{
                Serial.println("Packet Sent Successfully.");
            }
            
            delay(1);
        }
    }

    delay(10000);
}

void randomMac(){
    for(int i=0; i< 6; i++){
        macAdd[i] = random(256);
    }
}

void initSSID(){
    for(int i=0; i< 32; i++){
        emptySSID[i] = ' ';
    }
}