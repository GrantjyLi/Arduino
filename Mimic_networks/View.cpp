#include "View.h"

namespace View {
    void printInstruction(){
        Serial.print("\n");
        Serial.println("1: Observe networks.");
        Serial.println("2: Evil Twin on observed network");
        Serial.println("3: custom fake network");
        Serial.println("4: Beacon spam");

        Serial.print("Enter choice: ");
    }

    void printNetwork(String SSID, uint8_t strength, uint8_t numNetwork){
        Serial.println("--------------------------");
        Serial.printf("Network #: %d\n", numNetwork);
        Serial.printf("SSID: %s\n", SSID);
        Serial.printf("Strength: %d\n", strength);
    }
    
    void getStrInput(String& input){
        while (!Serial.available()){}
        input = Serial.readStringUntil('\n');// read the incoming data as string
        delay(100);
    }

    void getIntInput(uint8_t& input){
        while (!Serial.available()){}
        input = Serial.parseInt();
        delay(20);
    }
}
