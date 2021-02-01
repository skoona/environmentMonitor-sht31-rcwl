/**
 * @file main.cpp
 * @author James Scott, Jr.  aka(Skoona) (skoona@gmail.com)
 * @brief HomieNode featuring an SHT31 Temperature sensor.
 * @version 1.0.0
 * @date 2021-01-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <Arduino.h>
#include "Sht31Node.hpp"
#include "RCWLNode.hpp"

#ifdef ESP8266
extern "C"
{
#include "user_interface.h" // to set CPU Freq for Non-Huzzah's
}
#endif

/*
 * SKN_NODE_ID becomes the base name when ranges are used
 * ex: sknSensors/deviceName/SHT31_0/temperature -> 72.3 degress
 * Note: HomieNode(...range,lower,upper) manages this array suffix change; i.e no more name fixups
*/
#define SKN_MOD_NAME "Environment-SHT31-RCWL0516"
#define SKN_MOD_VERSION "1.0.6a"
#define SKN_MOD_BRAND "SknSensors"

#define SKN_TNODE_TITLE "SHT31 Temperature and Humidity Sensor"
#define SKN_MNODE_TITLE "RCWL-0516 Motion Sensor"

#define SKN_TNODE_TYPE "sensor"
#define SKN_MNODE_TYPE "sensor"

#define SKN_TNODE_ID "Ambient"    
#define SKN_MNODE_ID "Presence" 

// Select SDA and SCL pins for I2C communication and Motion
#define PIN_RCWL D5 // 14
#define PIN_SCL  D6 // 12
#define PIN_SDA  D7 // 13
#define SENSOR_READ_INTERVAL 300

HomieSetting<long> sensorsIntervalSetting("sensorInterval", "The interval in seconds to wait between sending commands.");
HomieSetting<long> motionIntervalSetting("motionHoldInterval", "The interval in seconds to hold motion detection.");

Sht31Node temperatureMonitor(PIN_SDA, PIN_SCL, SKN_TNODE_ID, SKN_TNODE_TITLE, SKN_TNODE_TYPE, SENSOR_READ_INTERVAL);
RCWLNode  motionMonitor(PIN_RCWL, SKN_MNODE_ID, SKN_MNODE_TITLE, SKN_MNODE_TYPE, SENSOR_READ_INTERVAL);

bool broadcastHandler(const String &level, const String &value)
{
  Homie.getLogger() << "Received broadcast level " << level << ": " << value << endl;
  return true;
}

void setup()
{  
  delay(50);  
  Serial.begin(115200);
  while( !Serial)
    delay(100); // wait for external monitor to ready

  Serial << endl << "Starting..." << endl;

  sensorsIntervalSetting.setDefaultValue(900).setValidator([](long candidate) {
    return (candidate >= 100) && (candidate <= 3000);
  });
  motionIntervalSetting.setDefaultValue(60).setValidator([](long candidate) {
    return (candidate >= 10) && (candidate <= 300);
  });

  temperatureMonitor.setMeasurementInterval(sensorsIntervalSetting.get());
  motionMonitor.setMotionHoldInterval(motionIntervalSetting.get());

  Homie_setFirmware(SKN_MOD_NAME, SKN_MOD_VERSION);
  Homie_setBrand(SKN_MOD_BRAND);

  Homie.setBroadcastHandler(broadcastHandler)
      .setup();
}

void loop()
{
  Homie.loop();
}
