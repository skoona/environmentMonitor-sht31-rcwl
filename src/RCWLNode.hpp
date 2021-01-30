/**
 * Homie Node for RCWL-0516 Doppler Radar Microwave Motion Sensor
 * 
 * GPIO pin for RCWL should have a pull down resistor to keep inactive state low
 * GPIO 16 has the INPUT_PULLDOWN_16 capability, but it is also the wakeup pin
 * 
 */

#pragma once

#include <Homie.hpp>

class RCWLNode : public HomieNode {

public:
  RCWLNode(const uint8_t rcwlPin, const char *id, const char *name, const char *nType, const int motionHoldInterval);

  void setMotionHoldInterval(unsigned long interval) { _motionHoldInterval = interval; }
  unsigned long getMotionHoldInterval() const { return _motionHoldInterval; }

protected:
  void setup() override;
  void loop() override;
  
private:
  int _motionPin = 0;

  // suggested rate is 1/60Hz (1m)
  static const int MIN_INTERVAL  = 10;  // in seconds
  static const int HOLD_INTERVAL = 60;

  const char *cCaption = "• RCWL-0516 Doppler Radar Microwave Motion Sensor:";
  const char* cIndent  = "  ◦ ";

  const char *cProperty     = "presence";
  const char *cPropertyName = "Presence";
  const char *cPropertyUnit = "%";

  unsigned long _motionHoldInterval;
  
  // RCWL-0516 Sensors address
  volatile bool _motion = false,
                _lastMotion = true;
  volatile byte _isrTrigger = LOW;
  volatile unsigned long _isrTriggeredAt = 0L;
 
};
