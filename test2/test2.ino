#include <ESP8266WiFi.h> //more about beacon frames https://mrncciew.com/2014/10/08/802-11-mgmt-beacon-frame/

const char * ssids[] = {
  "1",
  "2",
  "3",
  "4",
  "5",
  "6",
  "7"
};
uint8_t arrayindex =0;

const uint8_t channels[] = {1, 6, 11}; // used Wi-Fi channels
uint8_t channelIndex = 0;

uint8_t postSSID[13] = {
  0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, //supported rate
  0x03, 0x01, 0x04 /*DSSS (Current Channel)*/ };

extern "C" {
  #include "user_interface.h"
}

void setup() {
  delay(500);
  wifi_set_opmode(STATION_MODE);
  wifi_promiscuous_enable(1); 
}
void loop() {
    sendBeacon(ssids[arrayindex]);
    arrayindex++;
    if(arrayindex >= 7){arrayindex =0;}
}

void sendBeacon(const char* ssid) {
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

    int ssidLen = strlen(ssid);
    int packetSize = 51 + ssidLen; // Header + post + channel = 37 + 13 + 1

    packet[37] = ssidLen;

    memcpy(&packet[38], ssid, ssidLen);
    memcpy(&packet[38 + ssidLen], postSSID, 13);

    //random mac address
    for(int k=0; k< 6; k++){
        packet[10 + k] = packet[16 + k] = random(256);
    }

    for(int i=0; i< 3; i++){
      wifi_set_channel(channels[i]);

      packet[50 + ssidLen] = channels[i];

      for (int k = 0; k < 3; k++) {
          Serial.println(wifi_send_pkt_freedom(packet, packetSize, 0));
          delay(1);
      }
    }
}
