#ifndef BEACONSPAM_H
#define BEACONSPAM_H

#include <ESP8266WiFi.h>
#include <Arduino.h>

extern "C" {
    #include "user_interface.h"
}

#define NUM_CHANNELS 3
#define NUM_SSIDS 10 // change this to reflect the length of ssids array
#define WPA2 false



bool beaconSpamSetup();
void beaconSpam();


#endif