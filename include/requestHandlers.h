#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

#include <settings.h>
#include <hardwaredefs.h>
#include <debugOut.h>
#include "DuckyScript.h"

#include <zipWrapper.h>


void setRequestHandlers(AsyncWebServer *server);