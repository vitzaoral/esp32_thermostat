#ifndef __Thermostat_H
#define __Thermostat_H

#include <Arduino.h>
#include <MeteoData.h>
#include <EEPROM.h>

// relay pin
#define relayPinAddress 16
// minimal delay between two relay starts in seconds
#define heatingBreakTimeInSeconds 60

struct ThermostatStatus
{
  char *message;
  char *color;
  bool isHeating;
};

class Thermostat
{
public:
  static void initialize();
  static ThermostatStatus controllThermostat();
};

#endif