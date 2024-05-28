#include <ESP8266WiFi.h>
#include "View.h"

using namespace View;

#define MAXSSIDS 50

uint8_t numNetworks = 0;
String knownSSIDs[MAXSSIDS];

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

void setup() {

    WiFi.mode(WIFI_STA);
    WiFi.disconnect(); // Disconnect from any previously connected network

    delay(1000); // Wait for the module to disconnect

    Serial.begin(115200);
    Serial.flush();
    test();

}

void printInstruction(){
    Serial.println("\nEnter 1 to refresh for networks.");
    Serial.println("Enter 2 to mimic a found network SSID.");
    Serial.println("Enter 3 to create a unique network SSID.\n");
}

void loop(){
    delay(5000);
    printInstruction();
    while (!Serial.available()) {
        // Wait for user input
    }

    char input = Serial.read();
    switch (input){
        case '1':
            findNewNetworks();
            break;
        case '2':
            MimicNetwork();
            break;
        case '3':
            createUniqueAP(getSSIDInput());
            break;
        default:
            Serial.println("Enter a Valid Answer: ");
            printInstruction();
            break;
    }
}

void findNewNetworks(){
    Serial.println("\nScanning for available networks:");

    uint8_t newNumNetworks = WiFi.scanNetworks();

    for (uint8_t i = 0; i < newNumNetworks; i++){
        String newSSID = WiFi.SSID(i);
        bool newNetwork = true;

        for (uint8_t k = 0; k < numNetworks; k++){
            if(newSSID == knownSSIDs[k]){
            newNetwork = false;
            break;
            }
        }

        if(newNetwork){
            printNetwork(WiFi.SSID(i), WiFi.RSSI(i), numNetworks);
            knownSSIDs[numNetworks] = newSSID;
            numNetworks++;
        }
    }
}

void MimicNetwork(){
    Serial.println("Which network # to mimic: ");
    printAllNetwork();

    char input = Serial.read();

    if(input >= 0 && input < numNetworks){
        createUniqueAP(knownSSIDs[input]);
    }else{
        Serial.println("Invalid network number.");
    }
    
    
}

void createUniqueAP(String SSID){
    WiFi.softAPConfig(local_IP, gateway, subnet);
    WiFi.softAP(SSID);
}

void printAllNetwork(){
    for (uint8_t i = 0; i < numNetworks; i++){
        printNetwork(WiFi.SSID(i), WiFi.RSSI(i), i);
    }
    
}

void printNetwork(String SSID, uint8_t strength, uint8_t numNetwork){
    Serial.println("--------------------------");
    Serial.printf("Network #: %d\n", numNetwork);
    Serial.print("SSID: ");
    Serial.println(SSID);
    Serial.print("Strength: ");
    Serial.println(strength);
}

String getSSIDInput(){
    String inputString = "";
    while (Serial.available()) {
        char inChar = (char)Serial.read();

        inputString += inChar;

        if (inChar == '\n') {
            inputString.trim();
            break;
        }
    }
    return inputString;
}