#ifndef VIEW_H
#define VIEW_H

#include <Arduino.h> // Include Arduino for Serial
namespace View {
    void printInstruction();
    void printNetwork(String SSID, uint8_t strength, uint8_t numNetwork);
}

#endif
