#pragma once
#include <Arduino.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

#define SETTINGS_FILE "/settings/config.json"
#define SETTINGS_INFO_FILE "/settings/info.json"

extern JsonDocument settings;


void initSettings();
void storeSettings();
void loadSettings();
