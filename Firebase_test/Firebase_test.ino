#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>
#include<Firebase_ESP_Client.h>

//***REquired for firebase
#include <addons/TokenHelper.h> //Provide the token generation process info.
#include <addons/RTDBHelper.h> //Provide the RTDB payload printing info and other helper functions.

#define DEFAULT_PORT 80

#define SSID "notESP8266"
#define PASSWORD "pw4ESP8266"

IPAddress local_IP(192,168,4,255);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

#define FIREBASE_KEY "AIzaSyBLr1kYfHeiWlVLlcffqOU3xmJwlRYEvmI";
#define DB_URL "https://esp8266test-64f1b-default-rtdb.firebaseio.com/";

FirebaseData FBdata;
FirebaseAuth auth;
FirebaseConfig config;
bool signupOK = false;

ESP8266WebServer server(DEFAULT_PORT);

void setup(){
  Serial.begin(115200);
  delay(100);

  //setup soft access point
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(SSID, PASSWORD,1, false, 4);
  delay(100);

  //.on() accepts a URL path and function to handle it
  server.on("/", handleOnConnect);
  server.on("/submit", HTTP_POST, handleSubmit);
  //server.onNotFound(handleNotFound);

  server.begin();
  
  Serial.println("\nHTTP server started");
  Serial.printf("IP Address: ");
  Serial.println(WiFi.softAPIP());

  config.api_key = FIREBASE_KEY;
  config.database_url = DB_URL;
  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop(){
    // if(Firebase.ready() && signupOK){
    // }
    Serial.printf("Stations connected to soft-AP = %d\n", WiFi.softAPgetStationNum());
    delay(1000);
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
    server.send(200, "text/html", getHTMLForm());
}

String getHTMLForm(){
    String html = 
R"rawliteral(<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<meta name="viewport" content="width=device-width", initial-scale=1.0>
		<style>*{color: Black;size: 20px;}</style>
	</head>
	<body>
		<div id ="mainContainer">
			<h1>Hi friend, try edit me!</h1>
			<form id = "inputForm" action = "\submit" method = "post">
				<label for="passwordEnter">Password: (30 characters max)</label><br>
				<input type="text" class="inputBar" name="passwordEnter"><br><br>
				<input type="submit" value="Enter">
			</form>
		</div>
	</body>
</html>)rawliteral";

    return html;
}