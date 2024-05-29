#include <Arduino.h>

void setup(){
    Serial.begin(115200);
    while (!Serial) {
        ; // Wait for the serial port to connect. Needed for native USB port only
    }
}

void loop(){
    if (Serial.available() > 0) {
        String incomingData = Serial.readString(); // Read the incoming data as a string
        Serial.println(incomingData); // Print the received data back to the serial monitor
    }
}
