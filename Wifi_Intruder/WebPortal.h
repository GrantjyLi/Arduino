//for dealing with web page hosting and firebase interaction

#ifndef WEBPORTAL_H
#define WEBPORTAL_H

#include <ESP8266WebServer.h>
#include <DNSServer.h>

#define DEFAULT_PORT 80
#define DNS_PORT 53

extern ESP8266WebServer server;

namespace WebPortal{
    String getHTML(String&);
    void handleConnect();
    void handleSubmit();
    
}

#endif