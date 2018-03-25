# Window Cover HomeKit Apple
## Description
Control a Window cover motor from HomeKit by Apple.

## How to install:

### Brew:
 ` /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)”`

### Mosquitto:
```
`brew install mosquitto  `
`ln -sfv /usr/local/opt/mosquitto/*.plist ~/Library/LaunchAgents    `
`launchctl load ~/ Library/LaunchAgents/homebrew.mxcl.mosquitto.plist`
```

### Arduino: 
Download library: ArduinoJson, PubSubClient / Install NodeMCU package: 
http://arduino.esp8266.com/stable/package_esp8266com_index.json

### NodeJs: 
https://nodejs.org/en/download/
NPM: 
`sudo npm install -g homebridge / sudo npm install -g homebridge-mqtt`

### Settings: run home bridge in terminal, then close with CMD+C and add edit config.json with 
`sudo nano ~/.homebridge/config.json`

### Paste in config.json:
```
{
    "bridge": {
        "name": “Homebridge", // Name of the network
        "username": “CC:22:3D:E3:CE:30", // MAC Address
        "port": 51826, // Porta TCP 
        "pin": “031-45-154", // PIN code 
        "manufacturer": “artur", // Dev
        "model": “Homebridge", // Type
        "serialNumber": “0.4.20" // Version 
    },

    "accessories": [

    ],

    "platforms": [
          {
          "platform": “mqtt", // Do not change
          "name": “mqtt",// Do not change
          "url": “mqtt://192.168.5.158", // IP of the Broker MQTT
          "topic_type": “multiple", // Do not change
          "topic_prefix": “homebridge" // Do not change
        }
    ]
}
```

### On terminal:

```
 Add accessory: mosquitto_pub -h 192.168.5.158 -t homebridge/to/add -m '{"name": "cortina_quarto","service_name": "cortina","service": "WindowCovering","CurrentPosition": {"minStep": 5},"TargetPosition": {"minStep": 5},"CurrentHorizontalTiltAngle": {"minValue": 0,"minStep": 5},"TargetHorizontalTiltAngle": {"minValue": 0,"minStep": 5}}'
```

Monitor inputs: mosquitto_sub -t homebridge/from/response
Monitor outputs: mosquitto_sub -t homebridge/from/set

### Run python server: 
`sudo python run.py`

### Load code on ESPs: 
Change the SSID/PASSWORD and IP of the broker
