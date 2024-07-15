#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(172, 217, 28, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

String responseHTML = 
  R"rawliteral(<!DOCTYPE html><html lang='en'><head>
    <meta name='viewport' content='width=device-width'>
    <title>CaptivePortal</title></head><body>
    <h1>Hello World!</h1><p>This is a captive portal example.
      All requests will be redirected here.</p>
    <form action='/submit' method='post'>
    <label for='name'>Name:</label><br>
    <input type='text' id='name' name='name'><br><br>
    <input type='submit' value='Submit'>
    </form></body></html>)rawliteral";

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("bees");

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  // replay to all requests with same HTML
  webServer.onNotFound([]() {
    webServer.send(200, "text/html", responseHTML);
  });

  webServer.on("/submit", []() {
    if (webServer.method() == HTTP_POST) {
      String name = webServer.arg("name");
      Serial.println("Received name: " + name);
      webServer.send(200, "text/html", "<h1>Thank you, " + name + "!</h1><p>Your name has been recorded.</p>");
    } else {
      webServer.send(405, "text/html", "<h1>Method Not Allowed</h1>");
    }
  });

  webServer.begin();
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}