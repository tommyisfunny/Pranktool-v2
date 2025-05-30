#include <settings.h>
#include <debugOut.h>

JsonDocument settings;

void initSettings(){
  if(!userFS.exists(SETTINGS_FILE)){
    debugOutln("Settings file does not exist, creating new one");
    storeSettings();
  }

  loadSettings();

  JsonDocument settingsInfo;

  if(!deviceFS.exists(SETTINGS_INFO_FILE)){
    debugOutln("Settings info file does not exist!!!!");
    applyFallbackSettings();
    return;
  }

 
  File file = deviceFS.open(SETTINGS_INFO_FILE, "r");
  if (!file) {
    debugOutln("Failed to open settings info file for reading");
    applyFallbackSettings();
    return;
  }

  DeserializationError err = deserializeJson(settingsInfo, file);
  file.close();
  if (err) {
    debugOutln("Failed to parse settings info file");
    applyFallbackSettings();
    return;
  }

  bool settingsChanged = false;
  int settingsCount = settingsInfo["fields"].size();
  for(int i = 0; i < settingsCount; i++){
    String key = settingsInfo["fields"][i]["name"];
    if(!settings.containsKey(key)){
      settingsChanged = true;
      String defaultVal = settingsInfo["fields"][i]["default"];
      debugOutln("Settings file does not contain key: " + key + ", adding it with default value: " + defaultVal);
      settings[key] = settingsInfo["fields"][i]["default"];
    }
  }

  if(settingsChanged) storeSettings();
}



void storeSettings(){
  File file = userFS.open(SETTINGS_FILE, "w", true);
  if (!file) {
    debugOutln("Failed to open settings file for writing");
    return;
  }

  serializeJson(settings, file);
  file.close();

  debugOutln("Settings stored");
}


void loadSettings(){
  File file = userFS.open(SETTINGS_FILE, "r");
  if (!file) {
    debugOutln("Failed to open settings file for reading");
    return;
  }

  DeserializationError err = deserializeJson(settings, file);
  file.close();

  if (err) {
    debugOutln("Failed to parse settings file");
    return;
  }

  debugOutln("Settings loaded");
}

void applyFallbackSettings(){
  debugOutln("Applying fallback settings");
  settings["SSID"] = "Pranktool";
  settings["PASSWORD"] = "";
  settings["HIDDEN"] = false;
  settings["WLANONBOOT"] = true;
  settings["LEDSENABLED"] = true;
  storeSettings();
}

String getFirmwareVersion(){
  return String(VERSION);
}
String getCompileDate(){
  return String(COMPILE_DATE);
}