/*
   ESP32 Temperature Monitor
   Description:
     A simple ESP32 Temperature Monitor with Asysnc Web to display
     information. More to come!
   Notes:
   -Coming Soon!
   Contact Info:
   email - anthony.sleck@gmail.com
   web - anthony.sleck.us
   github - https://github.com/anthonysleck
   Changelog:
   0.1 - new code
   0.2 - add'd check before post of data to confirm greater than zero; add'd apache files
   0.4 - add'd DHT sensor data; rem'vs sql support
   0.5 - rmv'd DHT sensor; add'd SQL support back; revised all code; add'd debug option
   0.6 - add'd SEEED Studio XIAO C3; remv'd watchdog timer; add'd delay to SERIAL.BEGIN; add'd mDNS; remv'd REV from /TEMP webpage footer; remv'd APACHE/PCB path folders
   Contact Info:
   email - anthony.sleck@gmail.com
   web - anthony.sleck.us
   github - https://github.com/anthonysleck
*/

#ifndef config_h
#define config_h

// system versioning
#define VER "0.6"
#define VER_BUILD "053123"
#define email "anthony.sleck@gmail.com"
#define firmwareLink "https://github.com/anthonysleck/"

// common includes
#include <Arduino.h>
#include <AsyncElegantOTA.h>
#include <AsyncTCP.h>
#include <DallasTemperature.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <OneWire.h>
#include <SPI.h>
#include "webpages.h"
#include <WiFi.h>
#include <WiFiClient.h>

// debugging
#define DEBUG 1     // set to "0" to disable serial debugging

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif

// server configs
AsyncWebServer server(80);

// temperature
#define ONE_WIRE_BUS 5  // set to sensor port; GPIO
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
String ds_tempf;

// WiFi settings
const char* host = "ESPTempMonitor";  // change to the desired DNS name
const char *ssid = "********";        // enter your SSID here
const char *password = "********";    // enter password here

#endif
