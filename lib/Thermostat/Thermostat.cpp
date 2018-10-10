#include "Thermostat.h"

// relay pin
const int relayPinAddress = 16;

Thermostat::Thermostat()
{
    // set pinmodes to LED
    pinMode(relayPinAddress, OUTPUT);
    Serial.println("init thermostat");
}