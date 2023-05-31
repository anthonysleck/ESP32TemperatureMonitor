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
    // debugln(espTemperature);
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

void setup()
{
  // start serial
  Serial.begin(115200);
  Serial.setTxTimeoutMs(0);
  delay(5000);

  // sketch information
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

  // mDNS start
  if (!MDNS.begin(host))
  {
    debugln("Error setting up the mDNS responder!");
    while (1)
    {
      delay(1000);
    }    
  }
  debugln("mDNS responder started!");

  // start server
  rootServer();
}

void loop()
{
  // add code to loop here
}
