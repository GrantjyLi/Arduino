
#include <ESP8266WiFi.h>

extern "C" {
  #include "user_interface.h"
}

const uint8_t channels[] = {1, 6, 11};
const char* ssids[] = {"bogus", "queen", "boss"};
const char ssid[] = "cheese";

// Beacon Packet buffer
uint8_t packet[128] = { 0x80, 0x00, 0x00, 0x00, 
                /*4*/   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                /*10*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
                /*16*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 
                /*22*/  0xc0, 0x6c, 
                /*24*/  0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00, 
                /*32*/  0x64, 0x00, 
                /*34*/  0x01, 0x04, 
                /* SSID */
                /*36*/  0x00, 0x00, 0x72, 0x72, 0x72, 0x72, 0x72, 0x72,
                        0x01, 0x08, 0x82, 0x84,
                        0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03, 0x01, 
                /*56*/  0x04};                       


void setup() {
  delay(500);
  wifi_set_opmode(STATION_MODE);
  wifi_promiscuous_enable(1); 
}



void loop() {
    for(int i=0; i < 3 ; i++){
        wifi_set_channel(channels[i]);
        packet[56] = channels[i];

        // Randomize SRC MAC
        for(int k=0; k< 12; k++){
            packet[10 + k] = random(256);
        }

        for(int k=0; k<3; k++){
            packet[37] = strlen(ssids[k]) ;
            memcpy(&packet[38], ssids[k], strlen(ssids[k]) );
            
            wifi_send_pkt_freedom(packet, 57, 0);
        }
        // Serial.println(ssids[k]);
        // Serial.println(strlen(ssids[k]));
        // packet[37] = strlen(ssid);
        // memcpy(&packet[38], ssid, strlen(ssid));
        
        // wifi_send_pkt_freedom(packet, 57, 0);
        
    }
    delay(1);
}