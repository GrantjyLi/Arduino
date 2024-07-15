void setup() {
  // Start serial communication at 9600 baud rate
  Serial.begin(115200);
  // Give a little time for the serial monitor to initialize
  delay(1000);
}

void loop() {

    String inputString;

    getStrInput(inputString);
    inputString += 3;
    test(&inputString[0]);
  
  
  // Add a short delay to avoid overwhelming the serial buffer
  delay(100);
}

void getStrInput(String& input){
    while (!Serial.available()){}
    input = Serial.readStringUntil('\n');// read the incoming data as string
    delay(100);
}

void test(char* test){
  Serial.print("Received: ");
  Serial.println(test);
}
