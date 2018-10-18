#ifndef __Thermostat_H
#define __Thermostat_H

#include <Arduino.h>
#include <MeteoData.h>
#include <EEPROM.h>
#include <Display.h>

// relay pin
#define RELAY_PIN 16
// minimal delay between two relay starts in seconds
#define HEATING_BREAK_TIME_SECONDS 60
// interval when controll thermostat in seconds
#define CONTROLL_THERMOSTAT_INTERVAL 5

struct ThermostatStatus
{
  String message;
  int color;
  bool isHeating;
};

class Thermostat
{
public:
  static void initialize();
  static ThermostatStatus controllThermostat();
};

#endif