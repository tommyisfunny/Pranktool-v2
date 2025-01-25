#include <Arduino.h>
#include <SPIFFS.h>
#include <debugOut.h>


#include <FileHelper.h>
extern FileHelper fileHelper;

#include <DuckyScript.h>
#include <wlan.h>
#include <hardwaredefs.h>



String autostartPath = "";
String Button1Path = "";
String Button2Path = "";
String Button3Path = "";
String Button4Path = "";
String SSID = "";
String password = "";
bool hidden;

bool wlanActive = false;

void setupio() {
  for(int i = 11; i < 14; i++) pinMode(i, OUTPUT);

  pinMode(B_1, INPUT_PULLUP);
  pinMode(B_2, INPUT_PULLUP);
  pinMode(B_3, INPUT_PULLUP);
  pinMode(B_4, INPUT_PULLUP);
  pinMode(B_DARUN, INPUT_PULLUP);
  pinMode(B_SETUP, INPUT_PULLUP);

  keyboard.setBaseEP(3);
  keyboard.begin();
  duckyScript.begin();

  if (!CDCUSBSerial.begin()) Serial.println("Failed to start CDC USB stack");
  debugOutln("\n\nStarting Pranktool...\n");
  delay(1000); // time to get recognised
}

void processSetting(String line){
  int splitAt = line.indexOf(' ');
  String setting = line.substring(0, splitAt);
  String value = line.substring(splitAt);
  setting.trim();
  setting.toUpperCase();
  value.trim();

  debugOutln("Set " + setting + " to " + value);

  if(setting == "AUTOSTART") autostartPath = value; 
  else if(setting == "BUTTON1") Button1Path = value;
  else if(setting == "BUTTON2") Button2Path = value;
  else if(setting == "BUTTON3") Button3Path = value;
  else if(setting == "BUTTON4") Button4Path = value;
  else if(setting == "STANDARTDELAY") duckyScript.setStandartDelay(value.toInt());
  else if(setting == "SSID") SSID = value;
  else if(setting == "PASSWORD") password = value;
  else if(setting == "HIDDEN") hidden = (value == "true");
}

void updateSettings(){
  File fs = SPIFFS.open("/settings/config.cfg");
    String line = "";
    char _char = ' ';
    while(fs.available()){
        _char = fs.read();
        line += _char;
        if(_char != '\n') continue;
        processSetting(line);
        line = "";
    }
    if(line != "") processSetting(line);
    fs.close();
}

void writeSettings(String autostart, String Button1, String Button2, String Button3, String Button4, int StandartDelay, String SSID, String password, bool hidden){
  String settings = "AUTOSTART " + autostart + "\nBUTTON1 " + Button1 + "\nBUTTON2 " + Button2 + "\nBUTTON3 " + Button3 + "\nBUTTON4 " + Button4 + "\nSTANDARTDELAY " + String(StandartDelay);
  settings += "\nSSID " + SSID + "\nPASSWORD " + password + "\nHIDDEN " + (hidden ? "true" : "false");
  debugOutln("Write settings:\n" + settings);
  File fs = SPIFFS.open("/settings/config.cfg", FILE_WRITE);
  if (!fs) {
    debugOutln("There was an error opening the file for writing");
    return;
  }
  if (fs.print(settings)) {
    debugOutln("File was written");
  } else {
    debugOutln("File write failed");
  }
  fs.close();
}

void setup() {
  Serial.begin(115200);
  setupio();
  SPIFFS.begin(true);

  debugOut("SPIFFS info:\n");
  debugOut("Total:");
  debugOut(SPIFFS.totalBytes() / 1024);
  debugOut(" kb\nUsed:");
  debugOut(SPIFFS.usedBytes() / 1024);
  debugOut(" kb\nFree:");
  debugOut((SPIFFS.totalBytes() - SPIFFS.usedBytes()) / 1024);
  debugOut(" kb\n\n");

  updateSettings();

  if(digitalRead(B_DARUN) && autostartPath != "OFF") duckyScript.run("/payloads/" + autostartPath);
}

void loop() {
  // put your main code here, to run repeatedly:
  wlanloop();

  int b1 = !digitalRead(B_1);
  int b2 = !digitalRead(B_2);
  int b3 = !digitalRead(B_3);
  int b4 = !digitalRead(B_4);
  int setupB = !digitalRead(B_SETUP);

  if(setupB){
    if(wlanActive){
      stopWlan();
    }
    if(!wlanActive){
      setupWlan(); 
    }
    wlanActive = !wlanActive;
    digitalWrite(L_WLAN, wlanActive);
    delay(500);
  } 

  if(!b1 && !b2 && !b3 && !b4) return;

  digitalWrite(L_OK, HIGH);
  if(b1) duckyScript.run("/payloads/" + Button1Path);
  if(b2) duckyScript.run("/payloads/" + Button2Path);
  if(b3) duckyScript.run("/payloads/" + Button3Path);
  if(b4) duckyScript.run("/payloads/" + Button4Path);
  digitalWrite(L_OK, LOW);

  delay(100);
}

