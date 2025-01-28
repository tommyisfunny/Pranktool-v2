#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

#include <settings.h>
#include <hardwaredefs.h>
#include <debugOut.h>


extern DuckyScript duckyScript;

extern bool needToRunPayload;
extern String payloadToRun;


String editPayload = "";

AsyncWebServer server(80);

void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
  debugOutln(logmessage);

  if (!index) {
    logmessage = "Upload Start: " + String(filename);
    // open the file on first call and store the file handle in the request object
    request->_tempFile = SPIFFS.open("/payloads/" + filename, "w");
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

String processor(const String& var) {
  Serial.println("Processor: " + var);
  if(var == "FILECONTENT"){
    File fs = SPIFFS.open("/payloads/" + editPayload);
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
      request->send(SPIFFS, "/web/index.html");
    });

    server.on("/changeSettings", HTTP_GET, [] (AsyncWebServerRequest *request) {
        debugOutln("Change settings request");

        if(request->hasParam("autostart"))      settings["AUTOSTART"]     = request->getParam("autostart")->value();
        if(request->hasParam("button1"))        settings["BUTTON1"]       = request->getParam("button1")->value();
        if(request->hasParam("button2"))        settings["BUTTON2"]       = request->getParam("button2")->value();
        if(request->hasParam("button3"))        settings["BUTTON3"]       = request->getParam("button3")->value();
        if(request->hasParam("button4"))        settings["BUTTON4"]       = request->getParam("button4")->value();
        if(request->hasParam("standartDelay"))  settings["STANDARTDELAY"] = request->getParam("standartDelay")->value();
        if(request->hasParam("SSID"))           settings["SSID"]          = request->getParam("SSID")->value();
        if(request->hasParam("password"))       settings["PASSWORD"]      = request->getParam("password")->value();
        if(request->hasParam("hidden"))         settings["HIDDEN"]        = request->getParam("hidden")->value() == "true";
        if(request->hasParam("wlanonboot"))     settings["WLANONBOOT"]    = request->getParam("wlanonboot")->value() == "true";
        if(request->hasParam("ledsenabled"))    settings["LEDSENABLED"]   = request->getParam("ledsenabled")->value() == "true";
        
        storeSettings();
        duckyScript.setStandartDelay(settings["STANDARTDELAY"]);

        request->send(200, "text/plain", "OK");
    });

    server.on("/getSettings", HTTP_GET, [] (AsyncWebServerRequest *request) {
        debugOutln("Get settings request:");

        String settingsString;
        serializeJson(settings, settingsString);
        debugOutln(settingsString);
        request->send(200, "application/json", settingsString);
    });

    server.on("/getPayloads", HTTP_GET, [] (AsyncWebServerRequest *request) {
        debugOutln("Get payloads request:");
        String json = "[";
        File root = SPIFFS.open("/payloads");
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
        File root = SPIFFS.open("/payloads");
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
          SPIFFS.remove("/payloads/" + name);
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

        request->send(SPIFFS, "/web/edit.html", "text/html", false, processor);
    });

    server.on("/createPayload", HTTP_GET, [] (AsyncWebServerRequest *request) {
        debugOutln("Create payload request:");
        if(request->hasArg("name")){
          String name = request->getParam("name")->value();
          debugOutln("creating " + name);
          SPIFFS.open("/payloads/" + name, "w").close();
        }

        request->send(200, "plain/text", "ok");
    });

    server.serveStatic("/", SPIFFS, "/web/").setCacheControl("no-cache, no-store, must-revalidate");

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