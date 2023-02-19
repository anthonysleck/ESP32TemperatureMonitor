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
   0.2 - add'd check before post of data to confirm greater than zero; add'd apache files.
   0.4 - add'd DHT sensor data; rem'vs sql support
*/

// includes
#include "config.h"

// variables
float sensor_temperature, sensor_humidity, sensor_pressure, ds_temperature;
DHT dht(DHTPIN, DHTTYPE);

String readDHTemperature()
{
    // start DHT
  float espTemperature = dht.readTemperature(true);
  if (isnan(espTemperature)) {    
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(espTemperature);
    return String(espTemperature);
  }
}

String processor(const String &var)
{
  if (var == "ESPTEMPERATURE")
  {
    return readDHTemperature();
  }
  return String();
}

void IRAM_ATTR watchDogInterrupt()
{
  Serial.println("reboot");
  ESP.restart();
}

void watchDogRefresh()
{
  timerWrite(watchDogTimer, 0);   //reset timer
}

void startWatchdogTimer()
{
  Serial.println("Starting watchdog timer!");
  watchDogTimer = timerBegin(2, 80, true);
  timerAttachInterrupt(watchDogTimer, &watchDogInterrupt, true);
  timerAlarmWrite(watchDogTimer, WATCHDOG_TIMEOUT_S * 1000000, false);
  timerAlarmEnable(watchDogTimer);
  Serial.println("Watchdog timer started!");
}

void WiFiConnect()
{
  /*
  // start WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  */

  Serial.println();
  Serial.println();
  Serial.print("Starting WIFI AP with SSID of");
  Serial.println(ssid);
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
}

void rootServer()
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html, processor); });
             
  server.on("/temp", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", temp_html, processor); });

  server.on("/esptemperature", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", readDHTemperature().c_str()); });

  // start ElegantOTA
  AsyncElegantOTA.begin(&server);
  server.begin();
  Serial.println("HTTP server started");
}

void setup()
{
  delay(5000);

  // start serial
  Serial.begin(115200);

  // print sketch information
  Serial.println("Created by Anthony Sleck");
  Serial.println("Email at anthony.sleck@gmail.com");
  Serial.print("Version ");
  Serial.println(VER);
  Serial.print("Build Code ");
  Serial.println(VER_BUILD);
  Serial.print("Github: ");
  Serial.println(firmwareLink);

  // start DHT
  dht.begin();
  
  // start WiFi
  WiFiConnect();

  // start server
  rootServer();

  // start watchdog timer
  startWatchdogTimer();
}

void loop()
{
    // post to php-mysql
  unsigned long currentPostMillis = millis();
  if (currentPostMillis - startPostMillis >= periodPost)
  {
    float espTemperature = dht.readTemperature(true);
    Serial.println(espTemperature);
    delay(1000);
    
    //save the start millis()
    startPostMillis = currentPostMillis;
  }

  // reset watchdog timer
  watchDogRefresh();
}