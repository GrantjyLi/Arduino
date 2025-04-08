#include "View.h"

namespace View {
    void printInstruction(){
        Serial.print("\n");
        Serial.println("1: Observe networks.");
        Serial.println("2: Evil Twin on observed network");
        Serial.println("3: Custom AP");
        Serial.println("4: Beacon Spam");
        Serial.println("5: Deauth known Networks");

        Serial.print("Enter choice: ");
    }
    
    void getStrInput(String& input){
        while (!Serial.available()){} // wait for input to be free
        input = Serial.readStringUntil('\n');// read the incoming data as string
        while (Serial.available() > 0) {Serial.read();} // Clear the serial buffer
        delay(20);
        Serial.print("\n");
    }

    void getIntInput(uint8_t& input){
        while (!Serial.available()){} 
        input = Serial.parseInt();
        while (Serial.available() > 0) {Serial.read();}
        delay(20);
        Serial.print("\n");
        
    }
}
