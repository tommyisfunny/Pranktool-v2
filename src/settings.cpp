#include <settings.h>

String readSettings(const char *filePath, const String &key = "") {
  File file = SPIFFS.open(filePath, "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return "";
  }

  String content = file.readString();
  file.close();

  // If no specific key is requested, return full content
  if (key == "") {
    return content;
  }

  // Search for the specific key
  int start = 0;
  while (start < content.length()) {
    int end = content.indexOf('\n', start);
    if (end == -1) end = content.length();

    String line = content.substring(start, end);
    int separator = line.indexOf('=');

    if (separator != -1) {
      String currentKey = line.substring(0, separator);
      String currentValue = line.substring(separator + 1);

      currentKey.trim();
      currentValue.trim();

      if (currentKey == key) {
        Serial.println("Key found: " + key + " = " + currentValue);
        return currentValue; // Return the value for the specified key
      }
    }

    start = end + 1;
  }

  Serial.println("Key not found: " + key);

  // If key is not found, return an empty string
  return "";
}

// Write settings to a file
bool writeSettings(const char *filePath, const String &content) {
  File file = SPIFFS.open(filePath, "w");
  if (!file) {
    Serial.println("Failed to open file for writing");
    return false;
  }

  file.print(content);
  file.close();
  return true;
}

// Update specific setting in a file
bool updateSetting(const char *filePath, const String &key, const String &value) {
  Serial.println("Updating setting: " + key + " = " + value);

  String content = readSettings(filePath);

  // Parse settings into key-value pairs
  String updatedContent;
  bool keyFound = false;

  int start = 0;
  while (start < content.length()) {
    int end = content.indexOf('\n', start);
    if (end == -1) end = content.length();

    String line = content.substring(start, end);
    int separator = line.indexOf('=');

    if (separator != -1) {
      String currentKey = line.substring(0, separator);
      String currentValue = line.substring(separator + 1);

      currentKey.trim();
      currentValue.trim();

      if (currentKey == key) {
        currentValue = value; // Update the value
        keyFound = true;
      }

      updatedContent += currentKey + "=" + currentValue + "\n";
    }

    start = end + 1;
  }

  // If the key was not found, add it
  if (!keyFound) {
    updatedContent += key + "=" + value + "\n";
  }

  // Write back the updated content
  return writeSettings(filePath, updatedContent);
}
