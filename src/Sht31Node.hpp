/**
 * Homie Node for SHT31 Temperature and Humidity  Sensor
 * 
 */

#pragma once

#include <Homie.hpp>
#include <Wire.h>
#include <DFRobot_SHT3x.h>

class Sht31Node : public HomieNode {

public:
  Sht31Node(const uint8_t sclPin, const uint8_t sdaPin, const char *id, const char *name, const char *nType, const int measurementInterval);

  void          setMeasurementInterval(unsigned long interval) { _measurementInterval = interval; }
  unsigned long getMeasurementInterval() const { return _measurementInterval; }
  float getTemperatureF() const { return (_sensorResults.TemperatureF); }
  float getHumidity() const { return _sensorResults.Humidity; }

  // ((event.temperature * 1.8) + 32.0)

protected:
  void setup() override;
  void loop() override;
  
private:
  // SHT31 Sensors address
  static const int _sensorAddress = 0x44;  

  // suggested rate is 1/60Hz (1m)
  static const int MIN_INTERVAL         = 60;  // in seconds
  static const int MEASUREMENT_INTERVAL = 300;

  const char* cCaption = "• SHT31 Temperature & Humidity Sensor:";
  const char* cIndent  = "  ◦ ";

  const char* cTemperature     = "temperature";
  const char* cTemperatureName = "Temperature";
  const char* cTemperatureUnit = "°F";

  const char *cHumidity = "humidity";
  const char *cHumidityName = "Humidity";
  const char *cHumidityUnit = "%rH";

  unsigned long _measurementInterval;
  unsigned long _lastMeasurement;

  DFRobot_SHT3x *sensor;
  DFRobot_SHT3x::sRHAndTemp_t _sensorResults; // Sensor last temp/hum read
};
