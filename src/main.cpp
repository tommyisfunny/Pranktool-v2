#include <Arduino.h>
#include <SPIFFS.h>
#include <debugOut.h>
#include <FileHelper.h>

extern FileHelper fileHelper;

bool wlanActive = false;

bool needToRunPayload = false;
String payloadToRun = "";

#include <DuckyScript.h>
#include <wlan.h>
#include <hardwaredefs.h>
#include <settings.h>


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

  // CDCUSBSerial seems to mess with the keyboard
  //if (!CDCUSBSerial.begin()) Serial.println("Failed to start CDC USB stack");

  debugOutln("\n\nStarting Pranktool...\n");
  delay(1000); // time to get recognised
}

void printSPIFFSInfo(){
  debugOut("SPIFFS info:\n");
  debugOut("Total: ");
  debugOut(SPIFFS.totalBytes() / 1024);
  debugOut(" kb\nUsed: ");
  debugOut(SPIFFS.usedBytes() / 1024);
  debugOut(" kb\nFree: ");
  debugOut((SPIFFS.totalBytes() - SPIFFS.usedBytes()) / 1024);
  debugOutln(" kb\n");
}

void printESPInfo(){
  debugOutln("ESP info:");
  debugOut("Chip Model: ");
  debugOut(ESP.getChipModel());
  debugOut("\nChip Cores: ");
  debugOut(ESP.getChipCores());
  debugOut("\nCPU freq: ");
  debugOut(ESP.getCpuFreqMHz());
  debugOut(" Mhz\nFlash size: ");
  debugOut(ESP.getFlashChipSize() / 1024 / 1024);
  debugOutln(" MB\n");
}

void setup() {
  Serial.begin(115200);
  setupio();
  SPIFFS.begin(true);

  printSPIFFSInfo();
  printESPInfo();

  initSettings();


  duckyScript.setStandartDelay(settings["STANDARTDELAY"]);

  if(digitalRead(B_DARUN) && settings["AUTOSTART"] != "OFF")
  {
    if(settings["LEDSENABLED"]) digitalWrite(L_OK, HIGH);
    duckyScript.run("/payloads/" + settings["AUTOSTART"].as<String>());
    if(settings["LEDSENABLED"]) digitalWrite(L_OK, LOW);
  } 
  if(settings["WLANONBOOT"]){
    setupWlan();
    wlanActive = true;
    digitalWrite(L_WLAN, wlanActive && settings["LEDSENABLED"]);
  } 
}

void loop() {
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
    digitalWrite(L_WLAN, wlanActive && settings["LEDSENABLED"]);
    delay(500);
  } 

  if(needToRunPayload){
    if(settings["LEDSENABLED"]) digitalWrite(L_ERR, LOW);
    if(settings["LEDSENABLED"]) digitalWrite(L_OK, HIGH);
    duckyScript.run("/payloads/" + payloadToRun);
    if(settings["LEDSENABLED"]) digitalWrite(L_OK, LOW);
    needToRunPayload = false;
  }

  if(!b1 && !b2 && !b3 && !b4) return;

  if(settings["LEDSENABLED"]) digitalWrite(L_ERR, LOW);

  if(settings["LEDSENABLED"]) digitalWrite(L_OK, HIGH);
  if(b1) duckyScript.run("/payloads/" + settings["BUTTON1"].as<String>());
  if(b2) duckyScript.run("/payloads/" + settings["BUTTON2"].as<String>());
  if(b3) duckyScript.run("/payloads/" + settings["BUTTON3"].as<String>());
  if(b4) duckyScript.run("/payloads/" + settings["BUTTON4"].as<String>());
  if(settings["LEDSENABLED"]) digitalWrite(L_OK, LOW);

  delay(100);
}

