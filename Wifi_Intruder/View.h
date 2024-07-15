//to communicate to user using Serial terminal

#ifndef VIEW_H
#define VIEW_H

#include <Arduino.h> // Include Arduino for Serial
namespace View {
    void printInstruction();
    void getStrInput(String&);
    void getIntInput(uint8_t&);
}

#endif
