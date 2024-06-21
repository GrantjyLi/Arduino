#include <ESP8266WiFi.h>
#include "defs.h"

using namespace defs;

extern "C" {
    #include "user_interface.h"
    typedef void (*freedom_outside_cb_t)(uint8 status);
    int wifi_register_send_pkt_freedom_cb(freedom_outside_cb_t cb);
    void wifi_unregister_send_pkt_freedom_cb(void);
    int wifi_send_pkt_freedom(uint8 *buf, int len, bool sys_seq);
}

char emptySSID[32];
uint8_t macAdd[6];
uint8_t wifiChannel;
uint32_t packetSize;

char ssid[] = "cheese\n";

void setup(){
    packetSize = sizeof(beaconPacket);
    
    for(int i=0; i< 32; i++){
        emptySSID[i] = ' ';
    }

    Serial.begin(115200);
    Serial.println();
    
    WiFi.mode(WIFI_OFF); // No AP or STA mode
    wifi_set_opmode(STATION_MODE);

    if (WPA2) {
        beaconPacket[34] = 0x31;
    } else {
        beaconPacket[34] = 0x21;
        packetSize -= 26;
    }
}

void loop(){

    for(int i=0; i<NUM_BEACONS; i++){
        
        for(int k=0; k< 6; k++){
            macAdd[k] = random(256);
        }

        //copy fake mac address into the packet
        memcpy(&beaconPacket[10], macAdd, 6);
        memcpy(&beaconPacket[16], macAdd, 6);

        //reset and write ssid into the packet
        memcpy(&beaconPacket[38], emptySSID, 32);
        memcpy_P(&beaconPacket[38], ssid, sizeof(ssid));

        //switch wifi channels
        for(int k=0; k< sizeof(channels); k++){
            wifiChannel = channels[k];
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

