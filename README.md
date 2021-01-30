# EnvironmentMonitor-SHT31

HomieNode V3 featuring:
- SHT31 Temperature Class, using HomeNode, HomieRange, and Manual Assignment methods.
- RCWL-0516 Doppler Radar Microwave Motion Class, using HomieNode.

### Homie Config

```
{
  "name": "Environment Monitor",
  "device_id": "<device-location-name>",
  "device_stats_interval": 900,  
  "wifi": {
    "ssid": "<wifi-host>",
    "password": "<wifi-password>"
  },
  "mqtt": {
    "host": "<mqtt-hostname-or-ip>",
    "port": 1883,
	"base_topic": "sknSensors/",
    "auth": true,
    "username": "<mqtt-username>",
    "password": "<mqtt-password>"
  },
  "ota": {
    "enabled": true
  },
  "settings": {
      "sensorInterval": 900,
      "motionHoldInterval": 60
    }
}
```
