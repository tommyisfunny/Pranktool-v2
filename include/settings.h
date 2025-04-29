#pragma once
#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <version.h>

#define SETTINGS_FILE "/config.json"                // on the user partition
#define SETTINGS_INFO_FILE "/settings/info.json"    // on the device partition

extern JsonDocument settings;
extern fs::LittleFSFS deviceFS;
extern fs::LittleFSFS userFS;

void initSettings();
void storeSettings();
void loadSettings();
void applyFallbackSettings();

String getFirmwareVersion();
String getCompileDate();
