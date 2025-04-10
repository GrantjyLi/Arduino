
#include "BeaconSpam.h"

const uint8_t channels[] = {1, 6, 11}; // commonly used wifi channels on 2.4ghz

#define DEFAULT_NUM_SSIDS 8
char* spam_SSIDS[] = {
  "1) Never gonna give you Up", 
  "2) Never gonna let you Down", 
  "3) Never gonna run around",
  "4) And desert you", 
  "5) Never gonna make you cry", 
  "6) Never gonna say goodbye", 
  "7) Never gonna tell a lie",
  "8) Or hurt you"};


//beacon packet header
uint8_t beaconPacket[128] = {  
        0x80, 0x00, //Frame Control 
        0x00, 0x00, //Duration
/*4*/   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //Destination address 
/*10*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //Source address - overwritten later
/*16*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //BSSID - overwritten to the same as the source address
/*22*/  0xc0, 0x6c, //Seq-ctl
//Frame body starts here
/*24*/  0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00, //timestamp - the number of microseconds the AP has been active
/*32*/  0xFF, 0x00, //Beacon interval
/*34*/  0x21, 0x04, //Capability info
/* SSID */
/*36*/  0x00
};                       

uint8_t postSSID[13] = {
        0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, //supported rate
        0x03, 0x01, 0x04
};


bool beaconSpamSetup() {
    wifi_promiscuous_enable(1);
    return true;
}

void sendBeacon(char* ssids[], uint8_t numSSIDS){

    //randomize the source MAC address
    for(int k=0; k< 6; k++){
        beaconPacket[10 + k] = beaconPacket[16 + k] = random(256);
    }
    int ssidSize;
    int packetSize;
    uint32_t packetCount = 0;
    uint32_t time = millis();
    uint32_t lastTime = time;
    uint8_t arrayIndex = 0;

    Serial.println("Spamming SSIDs...");
    while(true){

        ssidSize = strlen(ssids[arrayIndex]);
        packetSize = 38 + ssidSize + sizeof(postSSID);

        beaconPacket[37] = ssidSize;

        //copying SSID into packet an post SSID
        memcpy(&beaconPacket[38], ssids[arrayIndex], ssidSize);
        memcpy(&beaconPacket[38 + ssidSize], postSSID, sizeof(postSSID));

        //looping through every wifi channel
        for(int i=0; i < NUM_CHANNELS ; i++){
            wifi_set_channel(channels[i]);
            beaconPacket[50 + ssidSize] = channels[i];

            packetCount += wifi_send_pkt_freedom(beaconPacket, packetSize, 0) == 0;  
            delay(5);
            
        }

        time = millis();
        if(time - lastTime > 50000){
            Serial.printf("Sent %d packets\n", packetCount);
            lastTime = time;
        }

        arrayIndex = (arrayIndex + 1) % numSSIDS;
    }
}

void defaultAttack(){
    sendBeacon(spam_SSIDS, DEFAULT_NUM_SSIDS);
}

void customAttack(){
    String customSSID;
    uint8_t numSSIDLimit;

    Serial.print("Enter Custom SSID to spam: ");
    View::getStrInput(customSSID);

    Serial.print("# of networks spammed: ");
    View::getIntInput(numSSIDLimit);

    char** spam_SSIDS = new char*[numSSIDLimit];

    for (uint8_t i = 0; i < numSSIDLimit; i++){
        String ssid = String(i+1) + ") " + customSSID + " ";
        spam_SSIDS[i] = new char[ssid.length() +1];
        memcpy(spam_SSIDS[i], ssid.c_str(), ssid.length() +1);
    }

    sendBeacon(spam_SSIDS, numSSIDLimit);
}

void beaconSpam() {
    uint8_t menuChoice;

    Serial.println("\nBeacon Spam Menu:\n");
    Serial.println("1: Default spam with ssids from file.");
    Serial.println("2: Custom SSID to spam.");

    View::getIntInput(menuChoice);
    
    switch(menuChoice){
        case 1:
            defaultAttack(); break;
        case 2:
            customAttack(); break;
        default:
            Serial.println("Invalid Input");
            break;
    }
}
