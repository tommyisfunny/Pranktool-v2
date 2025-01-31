#pragma once
#include <Arduino.h>
#include <myspiffs.hpp>
#include <ArduinoJson.h>

#define SETTINGS_FILE "/config.json"                // on the user partition
#define SETTINGS_INFO_FILE "/settings/info.json"    // on the device partition

extern JsonDocument settings;
extern SPIFFS deviceFS;
extern SPIFFS userFS;

void initSettings();
void storeSettings();
void loadSettings();
void applyFallbackSettings();