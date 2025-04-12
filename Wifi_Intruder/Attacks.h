#ifndef ATTACKS_H
#define ATTACKS_H

#include <ESP8266WiFi.h>
#include <Arduino.h>
#include "View.h"

extern "C" {
    #include "user_interface.h"
}

#define MAC_ADDR_LEN 6
#define DEAUTH_PACKET_SIZE 26

bool attacksSetup();
void beaconSpam();
void deauthNetwork(uint8_t, uint8_t*);

#endif