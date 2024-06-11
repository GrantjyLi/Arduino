#include "View.h"

namespace View {
    void printInstruction(){
        Serial.println("\nEnter 1 to refresh for networks.");
        Serial.println("Enter 2 to mimic a found network SSID.");
        Serial.println("Enter 3 to create a unique network SSID.\n");

        Serial.print("\nEnter choice: ");
    }

    void printNetwork(String SSID, uint8_t strength, uint8_t numNetwork){
        Serial.println("--------------------------");
        Serial.printf("Network #: %d\n", numNetwork);
        Serial.print("SSID: ");
        Serial.println(SSID);
        Serial.print("Strength: ");
        Serial.println(strength);
    }
    
    void getStrInput(String& input){
        while (!Serial.available()){}
        input = Serial.readStringUntil('\n');// read the incoming data as string
        delay(100);
    }

    void getIntInput(uint8_t& input){
        while (!Serial.available()){}
        input = Serial.parseInt();
        delay(100);
    }
}
