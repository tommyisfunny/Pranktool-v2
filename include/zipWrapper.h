#pragma once
#include <Arduino.h>
#include <LittleFS.h>
#include <debugOut.h>
#include <unzipLIB.h>

extern UNZIP zip;
extern fs::LittleFSFS deviceFS;

int unzipFile(fs::FS *fs, const char *zipFileName, const char *filename, const char *destDir, bool deleteZip = false);
int unzipAll(fs::FS *fs, const char *zipFileName, const char *destDir, bool deleteZip = false);
void testZip();
