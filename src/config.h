/*
   ESP32 Temperature Monitor
   Description:
     A simple ESP32 Temperature Monitor with Asysnc Web to display
     information. The data is also written to a SQL Server and
     an APACHE PHP webpage. There is also a simple PCB layout
     included to use a NARROW ESP32 dev board. More to come!
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
   Contact Info:
   email - anthony.sleck@gmail.com
   web - anthony.sleck.us
   github - https://github.com/anthonysleck
*/

#ifndef config_h
#define config_h

// system versioning
#define VER "0.5"
#define VER_BUILD "042023"
#define email "anthony.sleck@gmail.com"
#define firmwareLink "https://github.com/anthonysleck/"

// common includes
#include <Arduino.h>
#include <AsyncElegantOTA.h>
#include <AsyncTCP.h>
#include <DallasTemperature.h>
#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>
#include <OneWire.h>
#include <SPI.h>
#include "webpages.h"
#include <WiFi.h>
#include <WiFiClient.h>

// debugging
#define DEBUG 0     // set to "0" to disable serial debugging

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif


// server configs
AsyncWebServer server(80);

// post data timer
unsigned long startPostMillis;
const unsigned long periodPost = 300000;     // the time in ms the system will post data to the SQL server

//post data to WWW
const char* serverName = "http://********/post-data.php";
String apiKeyValue = "tPmAT5Ab3j7F9";

// temperature
#define ONE_WIRE_BUS 4  // set to sensor port; GPIO
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
String ds_tempf;

// wd timer
unsigned long startPostMillis;
const unsigned long periodPost = 300000; // the time in ms
#define WATCHDOG_TIMEOUT_S 15            // enter time in sec
hw_timer_t *watchDogTimer = NULL;

// WiFi settings
const char *ssid = "********";             // enter your SSID here
const char *password = "********"; // enter password here

#endif
