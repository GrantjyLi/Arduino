#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define DEFAULTPORT 80

const char* ssid = "notESP8266";
const char* pw = "pw4ESP8266";

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(DEFAULTPORT);

bool apStarted = false; // Flag to check if AP is started

void setup() {
    Serial.begin(115200);
    delay(100);
}

void loop() {
    // Check if AP is started, if not, start it
    if (!apStarted) {
        // Setup soft access point
        WiFi.softAPConfig(local_IP, gateway, subnet);
        WiFi.softAP(ssid, pw, 1, false, 4);
        delay(100);

        // Define handlers for web server routes
        server.on("/", handleOnConnect);
        server.on("/submit", HTTP_POST, handleSubmit);
        // server.onNotFound(handleNotFound);

        server.begin();
        apStarted = true; // Set the flag to indicate AP is started

        Serial.println("\nHTTP server started");
        Serial.printf("IP Address: ");
        Serial.println(WiFi.softAPIP());
    }

    // Handle client requests if AP is started
    if (apStarted) {
        server.handleClient();
        Serial.printf("Stations connected to soft-AP = %d\n", WiFi.softAPgetStationNum());
        delay(1000); // Short delay to avoid overwhelming the CPU
    }
}

// Function to handle client connection
void handleOnConnect() {
    Serial.println("Webpage visited connected.");
    sendHTML();
}

// Function to handle form submission
void handleSubmit() {
    if (server.hasArg("passwordEnter")) {
        String userPW = server.arg("passwordEnter");
        Serial.print("Password entered: ");
        Serial.println(userPW);
    } else {
        Serial.println("No password entered.");
    }
    sendHTML();
}

// Function to send HTML form to the client
void sendHTML() {
    server.send(200, "text/html", getHTMLForm());
}

// Function to generate HTML form
String getHTMLForm() {
    String html = R"rawliteral(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>*{color: Black; size: 20px;}</style>
</head>
<body>
    <div id="mainContainer">
        <h1>Hi friend, try edit me!</h1>
        <form id="inputForm" action="/submit" method="post">
            <label for="passwordEnter">Password: (30 characters max)</label><br>
            <input type="text" class="inputBar" name="passwordEnter"><br><br>
            <label for="ssidEnter">New SSID:</label><br>
            <input type="text" class="inputBar" name="ssidEnter"><br><br>
            <input type="submit" value="Enter">
        </form>
    </div>
</body>
</html>)rawliteral";

    return html;
}