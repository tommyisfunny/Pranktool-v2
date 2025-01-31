#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include <ArduinoJson.h>
#include <myspiffs.hpp>

#include <settings.h>
#include <hardwaredefs.h>
#include <debugOut.h>
#include "DuckyScript.h"


extern DuckyScript duckyScript;

extern bool needToRunPayload;
extern String payloadToRun;

extern SPIFFS userFS;
extern SPIFFS deviceFS;

String editPayload = "";

AsyncWebServer server(80);

void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
  debugOutln(logmessage);

  if (!index) {
    logmessage = "Upload Start: " + String(filename);
    // open the file on first call and store the file handle in the request object
    request->_tempFile = userFS.open("/payloads/" + filename, "w");
    debugOutln(logmessage);
  }

  if (len) {
    // stream the incoming chunk to the opened file
    request->_tempFile.write(data, len);
    logmessage = "Writing file: " + String(filename) + " index=" + String(index) + " len=" + String(len);
    debugOutln(logmessage);
  }

  if (final) {
    logmessage = "Upload Complete: " + String(filename) + ",size: " + String(index + len);
    // close the file handle as the upload is now done
    request->_tempFile.close();
    debugOutln(logmessage);
    request->redirect("/index.html");
  }
}

void handleSettingsUpload(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  String json = "";
  for (size_t i = 0; i < len; i++) {
    json += (char)data[i];
  }

  // Parse the JSON
  DeserializationError error = deserializeJson(settings, json);
  if (error) {
    debugOut("JSON parse failed: ");
    debugOutln(error.c_str());
    request->send(400, "text/plain", "JSON parse failed");
    return;
  }
  storeSettings();
  duckyScript.setStandartDelay(settings["STANDARTDELAY"]);

  request->send(200, "text/plain", "ok");
}

String processor(const String& var) {
  Serial.println("Processor: " + var);
  if(var == "FILECONTENT"){
    File fs = userFS.open("/payloads/" + editPayload);
    String fileContent = fs.readString();
    fs.close();
    fileContent.replace("%", "%%");
    return fileContent;
  }
  if(var == "FILENAME"){
    return editPayload;
  }

  return var;
}

void onOTAStart(){
  debugOutln("OTA update started...");
}

void onOTAProgress(size_t progress, size_t total){
  debugOut("Progress: ");
  debugOut(((int)progress/(int)total)*100);
  debugOutln("%");
}

void onOTAEnd(bool success){
  if(success){
    debugOutln("OTA update successful");
    debugOutln("Rebooting...");
  } else {
    debugOutln("OTA update failed");
  }
}

void setupWlan(){
    WiFi.mode(WIFI_AP);

    const char* ssid = settings["SSID"];
    const char* password = settings["PASSWORD"];
    bool hidden = settings["HIDDEN"];

    //const char* ssid = "test";
    //const char* password = "test1234";
    //bool hidden = false;

    if(password) WiFi.softAP(ssid, NULL    , 1, hidden?1:0);
    else         WiFi.softAP(ssid, password, 1, hidden?1:0);

    IPAddress IP = WiFi.softAPIP();
    debugOut("AP IP address: ");
    debugOutln(IP.toString());

    server.on("/", HTTP_GET, [] (AsyncWebServerRequest *request){
      request->send(deviceFS, "/web/index.html");
    });

    server.on("/saveSettings", HTTP_POST, [] (AsyncWebServerRequest *request) {
        debugOutln("Save settings request");
        request->send(200, "text/plain", "OK");
    }, NULL, handleSettingsUpload);

    server.on("/getSettings", HTTP_GET, [] (AsyncWebServerRequest *request) {
        debugOutln("Get settings request:");

        String settingsString;
        serializeJson(settings, settingsString);
        //debugOutln(settingsString);
        request->send(200, "application/json", settingsString);
    });

    server.on("/getSettingsInfo", HTTP_GET, [] (AsyncWebServerRequest *request) {
        debugOutln("Get info request:");

        File file = deviceFS.open(SETTINGS_INFO_FILE, "r");
        String infoString = file.readString();
        file.close();
        //debugOutln(infoString);
        request->send(200, "application/json", infoString);
    });

    server.on("/getPayloads", HTTP_GET, [] (AsyncWebServerRequest *request) {
        debugOutln("Get payloads request:");
        String json = "[";
        File root = userFS.open("/payloads");
        File file = root.openNextFile();

        while(file){
            String name = file.name();
            String ext = name.substring(name.lastIndexOf(".") + 1);
            /*if(ext == "dd")*/ json += "\"" + name + "\",";
            file = root.openNextFile();
        }
        json[json.length() - 1] = ']';
        debugOutln(json);

        request->send(200, "application/json", json);
    });

    server.on("/getOthers", HTTP_GET, [] (AsyncWebServerRequest *request) {
        debugOutln("Get others request:");
        String json = "[";
        File root = userFS.open("/payloads");
        File file = root.openNextFile();

        while(file){
            String name = file.name();
            String ext = name.substring(name.lastIndexOf(".") + 1);
            if(ext != "dd") json += "\"" + name + "\",";
            file = root.openNextFile();
        }
        json[json.length() - 1] = ']';
        debugOutln(json);

        request->send(200, "application/json", json);
    });

    server.on("/uploadPayload", HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200);
    }, handleUpload);

    server.on("/deletePayload", HTTP_GET, [] (AsyncWebServerRequest *request) {
        debugOutln("Delete payload request:");
        if(request->hasArg("payload")){
          String name = request->getParam("payload")->value();
          debugOutln("deleting " + name);
          userFS.remove("/payloads/" + name);
        }

        request->send(200, "plain/text", "ok");
    });

    server.on("/runPayload", HTTP_GET, [] (AsyncWebServerRequest *request) {
        debugOutln("Run payload request:");
        if(request->hasArg("payload")){
          String name = request->getParam("payload")->value();
          payloadToRun = name;
          needToRunPayload = true;
        }

        request->send(200, "plain/text", "ok");
    });

    server.on("/edit.html", HTTP_GET, [] (AsyncWebServerRequest *request) {
        debugOutln("Edit payloads request:");
        if(request->hasArg("payload")){
          String name = request->getParam("payload")->value();
          debugOutln("/payloads/" + name);
          editPayload = name;
        }

        request->send(deviceFS, "/web/edit.html", "text/html", false, processor);
    });

    server.on("/createPayload", HTTP_GET, [] (AsyncWebServerRequest *request) {
        debugOutln("Create payload request:");
        if(request->hasArg("name")){
          String name = request->getParam("name")->value();
          debugOutln("creating " + name);
          userFS.open("/payloads/" + name, "w").close();
        }

        request->send(200, "plain/text", "ok");
    });

    server.serveStatic("/", deviceFS, "/web/").setCacheControl("no-cache, no-store, must-revalidate");

    ElegantOTA.onStart(onOTAStart);
    ElegantOTA.onProgress(onOTAProgress);
    ElegantOTA.onEnd(onOTAEnd);

    ElegantOTA.begin(&server);
    server.begin();
}

void stopWlan(){
    server.end();
    WiFi.softAPdisconnect();
    WiFi.mode(WIFI_OFF);
}

void wlanloop() {
  ElegantOTA.loop();
}