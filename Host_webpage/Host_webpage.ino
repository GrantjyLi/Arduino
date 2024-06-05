#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>

#define DEFAULTPORT 80

const char* ssid = "notESP8266";
const char* pw = "pw4ESP8266";

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(DEFAULTPORT);

void setup(){
    Serial.begin(115200);
    delay(100);

    //setup soft access point
    WiFi.softAPConfig(local_IP, gateway, subnet);
    WiFi.softAP(ssid, pw,1, false, 4);
    delay(100);

    //.on() accepts a URL path and function to handle it
    server.on("/", handleOnConnect);
    server.on("/submit", HTTP_POST, handleSubmit);
    //server.onNotFound(handleNotFound);

    server.begin();
    
    Serial.println("\nHTTP server started");
    Serial.printf("IP Address: ");
    Serial.println(WiFi.softAPIP());

}

void loop(){
    server.handleClient();
    delay(1000);
    Serial.printf("Stations connected to soft-AP = %d\n", WiFi.softAPgetStationNum());
}

//when a device connects
void handleOnConnect(){
    Serial.println("Webpage visited connected.");
    sendHTML();

}

void handleSubmit(){

    if (server.hasArg("passwordEnter")) {
        String userPW = server.arg("passwordEnter");
        Serial.print("password entered: ");
        Serial.println(userPW);
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