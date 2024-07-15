//for dealing with web page hosting and firebase interaction

#ifndef WEBPORTAL_H
#define WEBPORTAL_H

#include "FirebaseCom.h"

#include <ESP8266WebServer.h>
#include <DNSServer.h>

#define DEFAULT_PORT 80
#define DNS_PORT 53

#define AP_SSID "notESP8266"
#define AP_PASSWORD "pw4ESP8266"

#define HOST_SSID "GPhone"
#define HOST_PASSWORD "hotspot4Grant"

extern ESP8266WebServer server;
extern String SSID;
extern bool internetConnection;

using namespace FirebaseCom;

namespace WebPortal{
    String getHTML(String&);
    void handleConnect();
    void handleSubmit();
    
}

#endif