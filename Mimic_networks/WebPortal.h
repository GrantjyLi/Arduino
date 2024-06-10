#ifndef WEBPORTAL_H
#define WEBPORTAL_H

#include<ESP8266WebServer.h>
#include<Firebase_ESP_Client.h>

#define DEFAULT_PORT 80

#define AP_SSID "notESP8266"
#define AP_PASSWORD "pw4ESP8266"

#define HOST_SSID "GPhone"
#define HOST_PASSWORD "hotspot4Grant"

#define FIREBASE_KEY "AIzaSyBLr1kYfHeiWlVLlcffqOU3xmJwlRYEvmI" //WEB API KEY, NOT APART OF SERVICE ACCOUNRTS
#define DB_URL "https://esp8266test-64f1b-default-rtdb.firebaseio.com"


extern ESP8266WebServer server;

namespace WebPortal{
    //String getHTML();
    //void handleConnect();
}

#endif