#include <ESP8266WiFi.h>
#include "View.h"
#include "WebPortal.h"

using namespace View;
using namespace WebPortal;

#define MAXSSIDS 50

uint8_t numNetworks = 0;
String knownSSIDs[MAXSSIDS];

ESP8266WebServer server(DEFAULT_PORT);

void setup() {

    WiFi.mode(WIFI_AP_STA);
    WiFi.disconnect(); // Disconnect from any previously connected network

    delay(1000); // Wait for the module to disconnect

    Serial.begin(115200);
    Serial.flush();

    //connecting to internet
    WiFi.begin(HOST_SSID, HOST_PASSWORD);
    while (WiFi.status() != WL_CONNECTED){
        Serial.println(".");
        delay(1000);
    }
    Serial.println("Connected To Internet");

    //setting up wifi AP
    WiFi.softAPConfig(local_IP, gateway, subnet);
    WiFi.softAP(AP_SSID, AP_PASSWORD,1, false, 4);
    delay(100);

    Serial.println("\nHTTP server started");
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());

    //.on() accepts a URL path and function to handle it
    server.on("/", handleConnect);
    //server.on("/submit", HTTP_POST, handleSubmit);
    //server.onNotFound(handleNotFound);

}


void loop(){

    printInstruction();
    while (!Serial.available()) {
        // Wait for user input
    }
    String inputSSID;

    char input = Serial.read();
    switch (input){
        case '1':
            findNewNetworks();
            break;
        case '2':
            MimicNetwork();
            break;
        case '3':
            Serial.print("Enter custom SSID: ");
            getStrInput(inputSSID);
            createUniqueAP(inputSSID);
            break;
        default:
            Serial.println("Enter a Valid Answer: ");
            printInstruction();
            break;
    }

    delay(1500);
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

    uint8_t networkNum;
    getIntInput(networkNum);
    Serial.println(networkNum);

    if(networkNum >= 0 && networkNum < numNetworks){
        createUniqueAP(knownSSIDs[networkNum]);
    }else{
        Serial.println("Invalid network number.");
    }
    
    
}

void createUniqueAP(String SSID){
    Serial.print("\nCreating custom network: ");
    Serial.println(SSID);
    
    WiFi.softAPConfig(local_IP, gateway, subnet);
    WiFi.softAP(SSID);
}

void printAllNetwork(){
    for (uint8_t i = 0; i < numNetworks; i++){
        printNetwork(WiFi.SSID(i), WiFi.RSSI(i), i);
    }
    
}