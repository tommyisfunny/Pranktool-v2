#include <debugOut.h>

// CDCUSBSerial seems to mess with the keyboard
//CDCusb CDCUSBSerial;

void debugOutln(String msg){
  Serial.println(msg);
  //CDCUSBSerial.println(msg);
}

void debugOutln(const char* msg){
  Serial.println(msg);
  //CDCUSBSerial.println(msg);
}

void debugOutln(int msg){
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