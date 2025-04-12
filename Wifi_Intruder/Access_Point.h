//for dealing with web page hosting and firebase interaction

#ifndef ACCESS_POINT_H
#define ACCESS_POINT_H

#include <ESP8266WebServer.h>
#include <DNSServer.h>

#define DEFAULT_PORT 80
#define DNS_PORT 53

extern IPAddress local_IP;
extern IPAddress subnet;
extern ESP8266WebServer server;
extern DNSServer dnsServer;

namespace Access_Point{
    String getHTML(String&);
    void handleConnect(String&);
    void handleSubmit(String&);
}

bool createAP(String = "Wifi Intruder AP");

#endif