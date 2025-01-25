#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

#include <hardwaredefs.h>

#include <debugOut.h>

extern void writeSettings(String autostart, String Button1, String Button2, String Button3, String Button4, int StandartDelay, String SSID, String password, bool hidden);
extern void updateSettings();
extern String autostartPath;
extern String Button1Path;
extern String Button2Path;
extern String Button3Path;
extern String Button4Path;
extern String SSID;
extern String password;
extern bool hidden;
extern DuckyScript duckyScript;

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
  if(var == "FILECONTENT"){
    File fs = SPIFFS.open("/payloads/" + editPayload);
    String fileContent = fs.readString();
    fs.close();
    return fileContent;
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

        if (
            request->hasParam("autostart") || request->hasParam("button1") || request->hasParam("button2") ||
            request->hasParam("button3")   || request->hasParam("button4") || request->hasParam("standartDelay") ||
            request->hasParam("SSID") || request->hasParam("password") || request->hasParam("hidden")
            ) {
            String newAutostart = autostartPath;
            String newButton1 = Button1Path;
            String newButton2 = Button2Path;
            String newButton3 = Button3Path;
            String newButton4 = Button4Path;
            int newStandartDelay = duckyScript.standartDelay;
            String newSSID = SSID;
            String newPassword = password;
            bool newHidden = hidden;

            if (request->hasParam("autostart"))     newAutostart = request->getParam("autostart")->value();
            if (request->hasParam("button1"))       newButton1 = request->getParam("button1")->value();
            if (request->hasParam("button2"))       newButton2 = request->getParam("button2")->value();
            if (request->hasParam("button3"))       newButton3 = request->getParam("button3")->value();
            if (request->hasParam("button4"))       newButton4 = request->getParam("button4")->value();
            if (request->hasParam("standartDelay")) newStandartDelay = request->getParam("standartDelay")->value().toInt();
            if (request->hasParam("SSID"))          newSSID = request->getParam("SSID")->value();
            if (request->hasParam("password"))      newPassword = request->getParam("password")->value();
            if (request->hasParam("hidden"))       newHidden = (request->getParam("hidden")->value() == "true");
            
            writeSettings(newAutostart, newButton1, newButton2, newButton3, newButton4, newStandartDelay, newSSID, newPassword, newHidden);
            updateSettings();
        }
        request->send(200, "text/plain", "OK");
    });

    server.on("/getSettings", HTTP_GET, [] (AsyncWebServerRequest *request) {
        debugOutln("Get settings request:");
        String json = "{";
        json += "\"autostart\":\"" + autostartPath + "\",";
        json += "\"button1\":\"" + Button1Path + "\",";
        json += "\"button2\":\"" + Button2Path + "\",";
        json += "\"button3\":\"" + Button3Path + "\",";
        json += "\"button4\":\"" + Button4Path + "\",";
        json += "\"standartDelay\":" + String(duckyScript.standartDelay) + ",";
        json += "\"SSID\":\"" + SSID + "\",";
        json += "\"password\":\"" + password + "\",";
        json += "\"hidden\":\""; 
        json += (hidden ? "true" : "false");
        json += "\"";
        json += "}";

        debugOutln(json);

        request->send(200, "application/json", json);
    });

    server.on("/getPayloads", HTTP_GET, [] (AsyncWebServerRequest *request) {
        debugOutln("Get payloads request:");
        String json = "[";
        File root = SPIFFS.open("/payloads");
        File file = root.openNextFile();

        while(file){
            json += "\"" + String(file.name()) + "\",";
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
          duckyScript.run("/payloads/" + name);
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