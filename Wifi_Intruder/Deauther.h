#ifndef DEAUTHER_H
#define DEAUTHER_H

#include <ESP8266WiFi.h>
#include <Arduino.h>

extern "C" {
    #include "user_interface.h"
}

#define MAC_ADDR_LEN 6
#define PACKET_SIZE 26

bool deauthSetup();
void deauthNetwork();

#endif