#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

#include <settings.h>
#include <hardwaredefs.h>

#include <debugOut.h>

extern String autostartPath;
extern String Button1Path;
extern String Button2Path;
extern String Button3Path;
extern String Button4Path;
extern String SSID;
extern String password;
extern bool hidden;
extern bool wlanonboot;
extern bool ledsenabled;
extern DuckyScript duckyScript;

extern bool needToRunPayload;
extern String payloadToRun;

extern void applySettings();

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

    if(password == "") WiFi.softAP(SSID.c_str(), NULL            , 1, hidden?1:0);
    else               WiFi.softAP(SSID.c_str(), password.c_str(), 1, hidden?1:0);

    IPAddress IP = WiFi.softAPIP();
    debugOut("AP IP address: ");
    debugOutln(IP.toString());

    server.on("/", HTTP_GET, [] (AsyncWebServerRequest *request){
      request->send(SPIFFS, "/web/index.html");
    });

    server.on("/changeSettings", HTTP_GET, [] (AsyncWebServerRequest *request) {
        debugOutln("Change settings request");

        if(request->hasParam("autostart"))      updateSetting("/settings/config.cfg", "AUTOSTART", request->getParam("autostart")->value());
        if(request->hasParam("button1"))        updateSetting("/settings/config.cfg", "BUTTON1", request->getParam("button1")->value());
        if(request->hasParam("button2"))        updateSetting("/settings/config.cfg", "BUTTON2", request->getParam("button2")->value());
        if(request->hasParam("button3"))        updateSetting("/settings/config.cfg", "BUTTON3", request->getParam("button3")->value());
        if(request->hasParam("button4"))        updateSetting("/settings/config.cfg", "BUTTON4", request->getParam("button4")->value());
        if(request->hasParam("standartDelay"))  updateSetting("/settings/config.cfg", "STANDARTDELAY", request->getParam("standartDelay")->value());
        if(request->hasParam("SSID"))           updateSetting("/settings/config.cfg", "SSID", request->getParam("SSID")->value());
        if(request->hasParam("password"))       updateSetting("/settings/config.cfg", "PASSWORD", request->getParam("password")->value());
        if(request->hasParam("hidden"))         updateSetting("/settings/config.cfg", "HIDDEN", request->getParam("hidden")->value());
        if(request->hasParam("wlanonboot"))     updateSetting("/settings/config.cfg", "WLANONBOOT", request->getParam("wlanonboot")->value());
        if(request->hasParam("ledsenabled"))    updateSetting("/settings/config.cfg", "LEDSENABLED", request->getParam("ledsenabled")->value());
        applySettings();

        request->send(200, "text/plain", "OK");
    });

    server.on("/getSettings", HTTP_GET, [] (AsyncWebServerRequest *request) {
        debugOutln("Get settings request:");

        JsonDocument doc;

        doc["autostart"] = autostartPath;
        doc["button1"] = Button1Path;
        doc["button2"] = Button2Path;
        doc["button3"] = Button3Path;
        doc["button4"] = Button4Path;
        doc["standartDelay"] = duckyScript.standartDelay;
        doc["SSID"] = SSID;
        doc["password"] = password;
        doc["hidden"] = hidden;
        doc["wlanonboot"] = wlanonboot;
        doc["ledsenabled"] = ledsenabled;

        String json;
        serializeJson(doc, json);

        debugOutln(json);

        request->send(200, "application/json", json);
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

    server.serveStatic("/", SPIFFS, "/web/");

    server.begin();
}

void stopWlan(){
    server.end();
    WiFi.softAPdisconnect();
    WiFi.mode(WIFI_OFF);
}

void wlanloop() {

}