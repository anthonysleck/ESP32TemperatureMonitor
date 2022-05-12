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
*/

// includes
#include "config.h"

// variables
String UploadData, timenow;
float sensor_temperature, sensor_humidity, sensor_pressure, ds_temperature;

String readDSTemperature()
{
  sensors.requestTemperatures();
  float espTemperature = sensors.getTempFByIndex(0);
  if (isnan(espTemperature))
  {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else
  {
    //Serial.println(espTemperature);
    return String(espTemperature);
  }
}

String processor(const String &var)
{
  if (var == "ESPTEMPERATURE")
  {
    return readDSTemperature();
  }
  else if (var == "REVISION")
  {
    return VER;
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
}

void temperatureSensors()
{
  sensors.requestTemperatures();
  float ds_temperature = sensors.getTempFByIndex(0);
  if (ds_temperature > 0)
  {
    ds_tempf = String(ds_temperature);
    
    // print current temperature data
    Serial.println("");
    Serial.print("Current Temperature is: ");
    Serial.print(ds_tempf);
    Serial.println("°F");
  }

  // reset watchdog timer
  watchDogRefresh();
}

void rootServer()
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html, processor); });
             
  server.on("/temp", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", temp_html, processor); });

  server.on("/esptemperature", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", readDSTemperature().c_str()); });

  server.on("/revision", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", VER); });

  // start ElegantOTA
  AsyncElegantOTA.begin(&server);
  server.begin();
  Serial.println("HTTP server started");
  /*
  ElegantOTA.begin(&serverOTA);
  server.begin();
  Serial.println("HTTP server started");
  */
}

void postData()
{
  // get current temperature data
  temperatureSensors();

  if (WiFi.status() == WL_CONNECTED)
  {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, serverName);

    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(ds_tempf) + "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);

    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0)
    {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else
    {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
  else
  {
    Serial.println("WiFi Disconnected");
  }

  // reset watchdog timer
  watchDogRefresh();
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

  // start WiFi
  WiFiConnect();

  // start server
  rootServer();

  // start watchdog timer
  startWatchdogTimer();

  // initial data to serial monitor and database
  postData();
}

void loop()
{
  // post to php-mysql
  unsigned long currentPostMillis = millis();
  if (currentPostMillis - startPostMillis >= periodPost)
  {
    postData();
    
    //save the start millis()
    startPostMillis = currentPostMillis;
  }

  // reset watchdog timer
  watchDogRefresh();
}