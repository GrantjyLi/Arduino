#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>

const char defaultPort = 80;

const char* ssid = "notESP8266";
const char* pw = "pw4ESP8266";

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(defaultPort);

void setup(){
    Serial.begin(115200);
    delay(100);

    //setup soft access point
    WiFi.softAPConfig(local_IP, gateway, subnet);
    WiFi.softAP(ssid, pw,1, false, 4);
    delay(100);

    //.on() accepts a URL path and function to handle it
    server.on("/", handleOnConnect);
    //server.onNotFound(handleNotFound);

    server.begin();
    
    Serial.println("HTTP server started");
    Serial.printf("IP Address: ");
    Serial.println(WiFi.softAPIP());

}

void loop(){
    server.handleClient();
}


//when a device connects
void handleOnConnect(){
    Serial.println("Device connected.");
    server.send(200, "text/html", sendHTMLForm());
}

String sendHTMLForm(){
    String html = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width\", initial-scale=1.0><style>*{color: Black;size: 20px;}</style></head><body><div id =\"mainContainer\"><h1>Hi friend, try edit me!</h1><form id = \"inputForm\"><label for=\"passwordEnter\">Password: (30 characters max)</label><br><input type=\"text\" id=\"passwordEnter\" name=\"fname\"><br><br><input type=\"submit\" value=\"Enter\"></form></div></body></html>";

    return html;
}