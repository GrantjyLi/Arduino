//https://randomnerdtutorials.com/esp8266-nodemcu-firebase-realtime-database/

#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>
#include<Firebase_ESP_Client.h>


//***REquired for firebase
#include "addons/TokenHelper.h" //Provide the token generation process info.
#include "addons/RTDBHelper.h" //Provide the RTDB payload printing info and other helper functions.

#include "View.h"
using namespace View;

#define DEFAULT_PORT 80

#define AP_SSID "notESP8266"
#define AP_PASSWORD "pw4ESP8266"

#define HOST_SSID "GPhone"
#define HOST_PASSWORD "hotspot4Grant"

#define FIREBASE_KEY "AIzaSyBLr1kYfHeiWlVLlcffqOU3xmJwlRYEvmI" //WEB API KEY, NOT APART OF SERVICE ACCOUNRTS
#define DB_URL "https://esp8266test-64f1b-default-rtdb.firebaseio.com"

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

FirebaseData FBdata;
FirebaseAuth auth;
FirebaseConfig config;
bool signupOK = false;

ESP8266WebServer server(DEFAULT_PORT);

void setup(){
  Serial.begin(115200);
  delay(100);

  //connecting to internet
  WiFi.begin(HOST_SSID, HOST_PASSWORD);
  while (WiFi.status() != WL_CONNECTED){
    Serial.println(".");
    delay(1000);
  }
  Serial.println("Connected To Internet");

  //setup soft access point
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(AP_SSID, AP_PASSWORD,1, false, 4);
  delay(100);
  
  Serial.println("\nHTTP server started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  config.api_key = FIREBASE_KEY;
  config.database_url = DB_URL;
  
  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("Sign up Confirmed");
    signupOK = true;
  }
  else{
    Serial.print("!ERROR: ");
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  //.on() accepts a URL path and function to handle it
  server.on("/", handleOnConnect);
  server.on("/submit", HTTP_POST, handleSubmit);
  //server.onNotFound(handleNotFound);

  server.begin();
}

void loop(){
    // if(Firebase.ready() && signupOK){
    // }
    Serial.printf("Stations connected to soft-AP = %d\n", WiFi.softAPgetStationNum());
    
    delay(5000);
    server.handleClient();
}

//when a device connects
void handleOnConnect(){
    Serial.println("Device connected.");
    sendHTML();
}

void handleSubmit(){

    if (server.hasArg("passwordEnter")) {
        String userPW = server.arg("passwordEnter");
        Serial.print("password entered: ");
        Serial.println(userPW);

        if(Firebase.RTDB.setString(&FBdata, "test/pw", userPW)){
            Serial.println("Password stored.");
        }
        else {
        Serial.println("Failed to store");
        Serial.println("REASON: " + FBdata.errorReason());
        }

    }else{
        Serial.println("No password entered.");
    }
    sendHTML();
}

void sendHTML(){
    server.send(200, "text/html", getHTML());
}

