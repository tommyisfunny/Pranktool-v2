#pragma once
#include <Arduino.h>

// CDCUSBSerial seems to mess with the keyboard
//#include <cdcusb.h>
//extern CDCusb CDCUSBSerial;

void debugOutln(String msg);
void debugOutln(const char* msg);
void debugOut(String msg);
void debugOut(const char* msg);
void debugOut(int msg);