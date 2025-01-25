#pragma once
#include <Arduino.h>
#include <SPIFFS.h>

String readSettings(const char *filePath, const String &key);
bool writeSettings(const char *filePath, const String &content);
bool updateSetting(const char *filePath, const String &key, const String &value);