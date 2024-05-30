void setup() {
  // Start serial communication at 9600 baud rate
  Serial.begin(115200);
  // Give a little time for the serial monitor to initialize
  delay(1000);
}

void loop() {
  // Check if data is available to read
  if (Serial.available() > 0) {
    // Read the incoming string until a newline character is found
    String inputString = Serial.readStringUntil('\n');
    
    // Print the received string to the serial monitor
    Serial.print("Received: ");
    Serial.println(inputString);
  }
  
  // Add a short delay to avoid overwhelming the serial buffer
  delay(100);
}
