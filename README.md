# esp_motion

I use this project to create a motion sensor that communicates over
WiFi with an MQTT broker.  My Home Assistant server subscribes to
messages from the ESP32 and can react when the doppler sensor detects
motion.

## Set up

Copy wifi-SAMPLE.h to wifi.h

Edit it to put in your WiFi credentials and your MQTT IP address,

Connect the motion sensor to the ESP32. Details here soon.

Upload it into a WeMOS R1 D1 or similar.

## What it does

On boot, connects to WiFI using credentials in wifi.h

## Hardware, currently

One of
* ESP32 Wrover
* WeMOS D1 R1 Arduino (based on the ESP8266)

* Doppler motion sensor ($1.95 from MPJA)

On the ESP32 I've got an antenna and a beeper and an LED.

## Infrastructure

I use Home Assistant and the Mosquitto MQTT broker.

## TODO

* Document how to configure in Home Assistant
* Remove unused MQTT PUB messages
* Do an ESP32 version
* Document the hookup. It's so simple almost not worth it. Almost. :-)
* Test on 24 VDC so it can be deployed on a led strip power supply
