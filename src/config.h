#ifndef config_h
#define config_h

// system versioning
#define VER "0.3"
#define VER_BUILD "05192022"
#define email "anthony.sleck@gmail.com"
#define firmwareLink "https://github.com/anthonysleck/ESP32TemperatureMonitor"

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

// post data timer
unsigned long startPostMillis;
const unsigned long periodPost = 300000;     // the time in ms the system will post data to the SQL server

//post data to WWW
const char* serverName = "http://********/post-data.php";
String apiKeyValue = "tPmAT5Ab3j7F9";

// temperature
#define ONE_WIRE_BUS 2  // set to sensor port
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
String ds_tempf;

// server configs
AsyncWebServer server(80);

// watchdog timer
#define WATCHDOG_TIMEOUT_S 15   // enter time in sec
hw_timer_t * watchDogTimer = NULL;

// WiFi credentials
const char *ssid = "********";     // enter your SSID here
const char *password = "********"; // enter password here

#endif