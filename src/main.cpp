#include <Arduino.h>
#include <SPIFFS.h>
#include <debugOut.h>


#include <FileHelper.h>
extern FileHelper fileHelper;

#include <DuckyScript.h>
#include <wlan.h>
#include <hardwaredefs.h>


#include <settings.h>



String autostartPath = "";
String Button1Path = "";
String Button2Path = "";
String Button3Path = "";
String Button4Path = "";
String SSID = "";
String password = "";
bool hidden;
bool wlanonboot = false;
bool ledsenabled = true;

bool wlanActive = false;

bool needToRunPayload = false;
String payloadToRun = "";

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

void applySettings(){
  autostartPath = readSettings("/settings/config.cfg", "AUTOSTART");
  Button1Path   = readSettings("/settings/config.cfg", "BUTTON1");
  Button2Path   = readSettings("/settings/config.cfg", "BUTTON2");
  Button3Path   = readSettings("/settings/config.cfg", "BUTTON3");
  Button4Path   = readSettings("/settings/config.cfg", "BUTTON4");
  duckyScript.setStandartDelay(readSettings("/settings/config.cfg", "STANDARTDELAY").toInt());
  SSID          = readSettings("/settings/config.cfg", "SSID");
  password      = readSettings("/settings/config.cfg", "PASSWORD");
  hidden        = readSettings("/settings/config.cfg", "HIDDEN") == "true";
  wlanonboot    = readSettings("/settings/config.cfg", "WLANONBOOT") == "true";
  ledsenabled   = readSettings("/settings/config.cfg", "LEDSENABLED") == "true";
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

  applySettings();

  if(digitalRead(B_DARUN) && autostartPath != "OFF")
  {
    if(ledsenabled) digitalWrite(L_OK, HIGH);
    duckyScript.run("/payloads/" + autostartPath);
    if(ledsenabled) digitalWrite(L_OK, LOW);
  } 
  if(wlanonboot){
    setupWlan();
    wlanActive = true;
    digitalWrite(L_WLAN, wlanActive && ledsenabled);
  } 
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
    digitalWrite(L_WLAN, wlanActive && ledsenabled);
    delay(500);
  } 

  if(needToRunPayload){
    if(ledsenabled) digitalWrite(L_OK, HIGH);
    duckyScript.run("/payloads/" + payloadToRun);
    if(ledsenabled) digitalWrite(L_OK, LOW);
    needToRunPayload = false;
  }

  if(!b1 && !b2 && !b3 && !b4) return;

  if(ledsenabled) digitalWrite(L_OK, HIGH);
  if(b1) duckyScript.run("/payloads/" + Button1Path);
  if(b2) duckyScript.run("/payloads/" + Button2Path);
  if(b3) duckyScript.run("/payloads/" + Button3Path);
  if(b4) duckyScript.run("/payloads/" + Button4Path);
  if(ledsenabled) digitalWrite(L_OK, LOW);

  delay(100);
}

