#pragma once
#include <Arduino.h>

#include "cdcusb.h"
CDCusb CDCUSBSerial;

void debugOutln(String msg){
  Serial.println(msg);
  //CDCUSBSerial.println(msg);
}

void debugOutln(const char* msg){
  Serial.println(msg);
  //CDCUSBSerial.println(msg);
}

void debugOut(String msg){
  Serial.print(msg);
  //CDCUSBSerial.print(msg);
}

void debugOut(const char* msg){
  Serial.print(msg);
  //CDCUSBSerial.print(msg);
}

void debugOut(int msg){
  Serial.print(msg);
  //CDCUSBSerial.print(msg);
}