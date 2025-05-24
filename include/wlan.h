#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>

#include <settings.h>
#include <debugOut.h>

#include <requestHandlers.h>

void setupWlan();
void stopWlan();
void wlanloop();
