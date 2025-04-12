//for dealing with web page hosting and firebase interaction

#ifndef WEBPORTAL_H
#define WEBPORTAL_H

#include <ESP8266WebServer.h>
#include <DNSServer.h>

#define DEFAULT_PORT 80
#define DNS_PORT 53

extern IPAddress local_IP;
extern IPAddress subnet;

extern ESP8266WebServer server;
extern DNSServer dnsServer;

namespace WebPortal{
    String getHTML(String&);
    void handleConnect(String&);
    void handleSubmit(String&);
}

bool createAP(String = "Wifi Intruder AP");

#endif