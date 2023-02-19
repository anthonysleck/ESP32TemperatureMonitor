#ifndef config_h
#define config_h

// system versioning
#define VER "0.4"
#define VER_BUILD "12212022"
#define email "anthony.sleck@gmail.com"
#define firmwareLink "https://github.com/anthonysleck/ESP32TemperatureMonitor"

// common includes
#include <Arduino.h>
#include <AsyncElegantOTA.h>
#include <AsyncTCP.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>
#include "webpages.h"
#include <WiFi.h>
#include <WiFiClient.h>

// server configs
AsyncWebServer server(80);

// post data timer
unsigned long startPostMillis;
const unsigned long periodPost = 300000;     // the time in ms the system will post data to the SQL server

// DHT setup
#define DHTPIN 14
#define DHTTYPE DHT11

// watchdog timer
#define WATCHDOG_TIMEOUT_S 15   // enter time in sec
hw_timer_t * watchDogTimer = NULL;

// WiFi settings
//const char WFIType "AP";                 //STA for station or AP for access point
const char *ssid = "SLEMPSONRACING";     // enter your SSID here
const char *password = "NotForYou"; // enter password here

#endif