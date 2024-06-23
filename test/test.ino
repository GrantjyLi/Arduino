
#include <ESP8266WiFi.h>

extern "C" {
  #include "user_interface.h"
}

const uint8_t channels[] = {1, 6, 11};
char* ssids[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven"};
int arrayindex;
// Beacon Packet buffer
uint8_t packet[128] = {  
        0x80, 0x00, //Frame Control 
        0x00, 0x00, //Duration
/*4*/   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //Destination address 
/*10*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //Source address - overwritten later
/*16*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //BSSID - overwritten to the same as the source address
/*22*/  0xc0, 0x6c, //Seq-ctl
//Frame body starts here
/*24*/  0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00, //timestamp - the number of microseconds the AP has been active
/*32*/  0xFF, 0x00, //Beacon interval
/*34*/  0x01, 0x04, //Capability info
/* SSID */
/*36*/  0x00
};                       

uint8_t postSSID[13] = {
        0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, //supported rate
        0x03, 0x01, 0x04
};

void setup() {
  delay(500);
  wifi_set_opmode(STATION_MODE);
  wifi_promiscuous_enable(1); 
  Serial.begin(115200);
  Serial.println("");
  arrayindex =0;
}

void sendBeacon(char* ssid){
    int ssidSize = strlen(ssid);
    int packetSize = 38 + ssidSize + sizeof(postSSID);

    packet[37] = ssidSize;

    //copying SSID into packet an post SSID
    memcpy(&packet[38], ssid, ssidSize);
    memcpy(&packet[38 + ssidSize], postSSID, sizeof(postSSID));
    
    //looping through every wifi channel
    for(int i=0; i < 3 ; i++){
        wifi_set_channel(channels[i]);
        packet[50 + ssidSize] = channels[i];

        // Randomize SRC MAC
        for(int k=0; k< 6; k++){
            packet[10 + k] = packet[16 + k] = random(256);
        }
        
        //sending packets
        // if(wifi_send_pkt_freedom(packet, packetSize, 0) != 0){
        //     Serial.print("Failed to send: ");
        //     Serial.println(ssid);
        // }else{
        //     Serial.println("Packet sent successfully");
        //     wifi_send_pkt_freedom(packet, packetSize, 0);
        //     wifi_send_pkt_freedom(packet, packetSize, 0);
        // }

        for(int i=0; i<3; i++){
            wifi_send_pkt_freedom(packet, packetSize, 0);
            delay(20);
        }
        
    }
    
}

void loop() {
    
    sendBeacon(ssids[arrayindex]);
    arrayindex++;
    if(arrayindex >= 11){arrayindex =0;}
}