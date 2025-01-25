#pragma once
#include "Arduino.h"
#include "FS.h"

#ifndef FILEHELPER_H
  #define FILEHELPER_H

class FileHelper {
public:
  bool debugOutput = true;

  void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if (!root) {
      Serial.println("- failed to open directory");
      return;
    }
    if (!root.isDirectory()) {
      Serial.println("- not a directory");
      return;
    }

    File file = root.openNextFile();
    while (file) {
      if (file.isDirectory()) {
        Serial.print("  DIR : ");
        Serial.println(file.name());
        if (levels) {
          listDir(fs, file.name(), levels - 1);
        }
      } else {
        Serial.print("  FILE: ");
        Serial.print(file.name());
        Serial.print("\tSIZE: ");
        Serial.println(file.size());
      }
      file = root.openNextFile();
    }
  }

  bool exists(fs::FS &fs, const char *path){
    return fs.exists(path);
  }

  String readFile(fs::FS &fs, const char *path) {
    if(debugOutput) Serial.printf("## Reading file: %s\r\n", path);

    File file = fs.open(path);
    if (!file || file.isDirectory()) {
      if(debugOutput) Serial.println("## - failed to open file for reading");
      return "";
    }
    String res = "";
    while (file.available()) {
      res += (char)file.read();
    }
    return res;
  }

  void writeFile(fs::FS &fs, const char *path, const char *message) {
    if(debugOutput) Serial.printf("## Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file) {
      if(debugOutput) Serial.println("## - failed to open file for writing");
      return;
    }
    if (file.print(message)) {
      if(debugOutput) Serial.println("## - file written");
    } else {
      if(debugOutput) Serial.println("## - frite failed");
    }
  }

  void appendFile(fs::FS &fs, const char *path, const char *message) {
    if(debugOutput) Serial.printf("## Appending to file: %s\r\n", path);

    File file = fs.open(path, FILE_APPEND);
    if (!file) {
      if(debugOutput) Serial.println("## - failed to open file for appending");
      return;
    }
    if (file.print(message)) {
      if(debugOutput) Serial.println("## - message appended");
    } else {
      if(debugOutput) Serial.println("## - append failed");
    }
  }

  void renameFile(fs::FS &fs, const char *path1, const char *path2) {
    if(debugOutput) Serial.printf("## Renaming file %s to %s\r\n", path1, path2);
    if (fs.rename(path1, path2)) {
      if(debugOutput) Serial.println("## - file renamed");
    } else {
      if(debugOutput) Serial.println("## - rename failed");
    }
  }

  void deleteFile(fs::FS &fs, const char *path) {
    if(debugOutput) Serial.printf("## Deleting file: %s\r\n", path);
    if (fs.remove(path)) {
      if(debugOutput) Serial.println("## - file deleted");
    } else {
      if(debugOutput) Serial.println("## - delete failed");
    }
  }

  void mkDir(fs::FS &fs, const char *path){
    if(debugOutput) Serial.printf("## create dir: %s\r\n", path);
    if (fs.mkdir(path)) {
      if(debugOutput) Serial.println("## - dir created");
    } else {
      if(debugOutput) Serial.println("## - dir creation failed");
    }
  }
};

#endif