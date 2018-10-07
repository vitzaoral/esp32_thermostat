#include "Thermostat.h"

// relay pin
const int relayPinAddress = 5;

void Thermostat::initialize()
{
    // set pinmodes to LED
    pinMode(relayPinAddress, OUTPUT);
}