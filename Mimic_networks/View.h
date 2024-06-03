#ifndef VIEW_H
#define VIEW_H

#include <Arduino.h> // Include Arduino for Serial
namespace View {
    void printInstruction();
    void printNetwork(String, uint8_t, uint8_t);
    void getStrInput(String&);
    void getIntInput(uint8_t&);
}

#endif
