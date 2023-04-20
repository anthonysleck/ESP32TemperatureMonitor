// includes
#include "config.h"

String readDSTemperature()
{
  sensors.requestTemperatures();
  float espTemperature = sensors.getTempFByIndex(0);
  if (isnan(espTemperature))
  {
    debugln("Failed to read from DHT sensor!");
    return "--";
  }
  else
  {
    //debugln(espTemperature);
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
  debugln("reboot");
  ESP.restart();
}

void watchDogRefresh()
{
  timerWrite(watchDogTimer, 0);   //reset timer
}

void startWatchdogTimer()
{
  debugln("Starting watchdog timer!");
  watchDogTimer = timerBegin(2, 80, true);
  timerAttachInterrupt(watchDogTimer, &watchDogInterrupt, true);
  timerAlarmWrite(watchDogTimer, WATCHDOG_TIMEOUT_S * 1000000, false);
  timerAlarmEnable(watchDogTimer);
  debugln("Watchdog timer started!");
}

void temperatureSensors()
{
  sensors.requestTemperatures();
  float ds_temperature = sensors.getTempFByIndex(0);
  if (ds_temperature > 0)
  {
    ds_tempf = String(ds_temperature);
    
    // print current temperature data
    debugln("");
    debug("Current Temperature is: ");
    debug(ds_tempf);
    debugln("°F");
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

  // start ElegantOTA
  AsyncElegantOTA.begin(&server);
  server.begin();
  debugln("HTTP server started");
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
    debug("httpRequestData: ");
    debugln(httpRequestData);

    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0)
    {
      debug("HTTP Response code: ");
      debugln(httpResponseCode);
    }
    else
    {
      debug("Error code: ");
      debugln(httpResponseCode);
    }
    http.end();
  }
  else
  {
    debugln("WiFi Disconnected");
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
  debugln("Created by Anthony Sleck");
  debugln("Email at anthony.sleck@gmail.com");
  debug("Version ");
  debugln(VER);
  debug("Build Code ");
  debugln(VER_BUILD);
  debug("Github: ");
  debugln(firmwareLink);

  // start WiFi
  debugln("Starting WIFI connection!");
  debugln();
  debug("Connecting to ");
  debugln(ssid);
  WiFi.begin(ssid, password);
  debugln("");

  while (WiFi.status() != WL_CONNECTED)
  {
      delay(1000);
      debug(".");
  }

  debugln("");
  debug("Connected to ");
  debugln(ssid);
  debug("IP address: ");
  debugln(WiFi.localIP());

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
