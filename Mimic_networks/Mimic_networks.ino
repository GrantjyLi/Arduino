#include <ESP8266WiFi.h>

#define MAXSSIDS 50

uint8_t numNetworks = 0;
String knownSSIDs[MAXSSIDS];

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

void setup() {
    Serial.begin(115200);
    Serial.flush();
    printInstruction();

    WiFi.mode(WIFI_STA);
    WiFi.disconnect(); // Disconnect from any previously connected network

    delay(1000); // Wait for the module to disconnect

    Serial.println("\nScanning for available networks:");
    numNetworks = WiFi.scanNetworks();

    if (numNetworks == 0) {
    Serial.println("No networks found.");
    } else {
    Serial.println("Networks found:");
    for (int i = 0; i < numNetworks; i++) {
        knownSSIDs[i] = WiFi.SSID(i);
        Serial.println("--------------------------");
        Serial.printf("Network #: %d\n", i);
        Serial.print("SSID: ");
        Serial.println(WiFi.SSID(i));
        Serial.print("Strength: ");
        Serial.println(WiFi.RSSI(i));
    }
    }


}

void printInstruction(){
    Serial.println("Enter 1 to refresh for networks.");
    Serial.println("Enter 2 to mimic a found network SSID.");
    Serial.println("Enter 3 to create a unique network SSID.\n");
}

void loop(){
    delay(5000);
    char input = getInput();
    switch (input){
        case 1:
            findNewNetworks();
            break;
        case 2:
            MimicNetwork();
            break;
        case 3:
            createUniqueAP();
            break;
        default:
            break;
    }

}

char getInput(){
    if (Serial.available() > 0) {
        char input = Serial.read();
        return input;
    }
    return 0;
}

void findNewNetworks(){
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
        Serial.println("--------------------------");
        Serial.printf("Network #: %d\n", numNetworks);
        Serial.print("SSID: ");
        Serial.println(newSSID);
        Serial.print("Strength: ");
        Serial.println(WiFi.RSSI(i));

        knownSSIDs[numNetworks] = newSSID;
        numNetworks++;
    }
    }
}

void MimicNetwork(){

}

void createUniqueAP(String SSID){
    WiFi.softAPConfig(local_IP, gateway, subnet);
    WiFi.softAP(SSID);
}
