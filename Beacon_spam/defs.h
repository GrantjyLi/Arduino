#ifndef DEFS_H
#define DEFS_H

#define NUM_BEACONS 10
#define WPA2 false

namespace defs{
    const uint8_t channels[] = {1, 6, 11}; // commonly used wifi channels on 2.4ghz
    char* ssids[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven"};
        uint8_t packet[128] = {  
            0x80, 0x00, //Frame Control 
            0x00, 0x00, //Duration
    /*4*/   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //Destination address 
    /*10*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //Source address - overwritten later
    /*16*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //BSSID - overwritten to the same as the source address
    /*22*/  0xc0, 0x6c, //Seq-ctl
    //Frame body starts here
    /*24*/  0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00, //timestamp - the number of microseconds the AP has been active
    /*32*/  0xFF, 0x00, //Beacon interval
    /*34*/  0x21, 0x04, //Capability info
    /* SSID */
    /*36*/  0x00
    };                       
        // Beacon Packet buffer
    uint8_t postSSID[13] = {
            0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, //supported rate
            0x03, 0x01, 0x04
    };
}

#endif