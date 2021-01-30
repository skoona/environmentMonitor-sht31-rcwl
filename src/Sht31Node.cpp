/**
 * Homie Node for SHT31 Temperature and Humidity  Sensor
 * 
 */
#include "Sht31Node.hpp"

Sht31Node::Sht31Node(const uint8_t sdaPin, const uint8_t sclPin, const char *id, const char *name, const char *nType, const int measurementInterval = MEASUREMENT_INTERVAL)
    : HomieNode(id, name, nType)
{
  _measurementInterval = (measurementInterval > MIN_INTERVAL) ? measurementInterval : MIN_INTERVAL;
  _lastMeasurement     = 0;

  // Start up the library
  Wire.begin(sdaPin, sclPin);
  sensor = new DFRobot_SHT3x(&Wire, _sensorAddress);
  sensor->begin();
}

  /**
    * Called by Homie when Homie.setup() is called; Once!
  */
  void Sht31Node::setup() {
    Homie.getLogger() << cIndent << F("Sensor SerialNumber: ") << sensor->readSerialNumber() << endl;
    Homie.getLogger() << cIndent << F("Sensor Heater Status: ") << (sensor->softReset() ? "Heater Off" : "Heater On") << endl;

    advertise(cHumidity)
        .setName(cHumidityName)
        .setDatatype("float")
        .setUnit(cHumidityUnit);

    advertise(cTemperature)
      .setName(cTemperatureName)
      .setDatatype("float")
      .setUnit(cTemperatureUnit);
  }

  /**
   * Called by Homie when homie is connected and in run mode
  */
  void Sht31Node::loop() {
    if (!(millis() - _lastMeasurement >= _measurementInterval * 1000UL || _lastMeasurement == 0)) {
      return;
    }
    _lastMeasurement = millis();

    Homie.getLogger() << F("〽 Sending Temperature: ") << getId() << endl;
    
    _sensorResults = sensor->readTemperatureAndHumidity(sensor->eRepeatability_High);
    if (_sensorResults.ERR == ERR_OK) {
      Homie.getLogger() << cIndent
                        << F("Temperature=")
                        << getTemperatureF()
                        << F(", Humidity=")
                        << getHumidity()
                        << endl;
      setProperty(cTemperature).send(String( getTemperatureF() ));
      setProperty(cHumidity).send(String( getHumidity() ));
    }
    else
    {
      Homie.getLogger() << cIndent
                        << F("✖ Error reading sensor: ")
                        << _sensorResults.ERR
                        << ", value (F) read=" << _sensorResults.TemperatureF
                        << endl;
    }
  }

