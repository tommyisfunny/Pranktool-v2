#include <requestHandlers.h>

extern bool needToRunPayload;
extern String payloadToRun;

extern FileHelper fileHelper;

extern fs::LittleFSFS userFS;
extern fs::LittleFSFS deviceFS;


String editPayload = "";


// get the current settings
void getSettingsHandler(AsyncWebServerRequest *request) {
    debugOutln("Get settings request:");

    String settingsString;
    serializeJson(settings, settingsString);
    //debugOutln(settingsString);
    request->send(200, "application/json", settingsString);
}

// save the current settings to the filesystem
void saveSettingsHandler(AsyncWebServerRequest *request) {
    debugOutln("Save settings request");
    request->send(200, "text/plain", "OK");
}
// handle the settings file upload
void settingsFileUploadHandler(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
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

// get the settings info
void getSettingsInfoHandler(AsyncWebServerRequest *request) {
       debugOutln("Get info request:");

       File file = deviceFS.open(SETTINGS_INFO_FILE, "r");
       String infoString = file.readString();
       file.close();
       //debugOutln(infoString);
       request->send(200, "application/json", infoString);
    }

// get the available payloads
void getPayloadsHandler(AsyncWebServerRequest *request) {
    debugOutln("Get payloads request:");
    JsonDocument json;
    File root = userFS.open("/payloads/");
    File dir = root.openNextFile();

    String name;
    String path;

    while(dir){
        path = dir.path();
        name = dir.name();

        //debugOutln(path + ":");

        if(dir.isDirectory()){
          File file = dir.openNextFile();
          while(file){
            //debugOutln(file.name());
            json[name].add(file.name());
            file = dir.openNextFile();
          }
        }
        dir = root.openNextFile();
    }
        
    String jsonString;
    serializeJson(json, jsonString);
    //debugOutln(jsonString);

    request->send(200, "application/json", jsonString);
    }

// get a payload file
void getPayloadFileHandler(AsyncWebServerRequest *request) {
    debugOutln("Get file request:");
    if(request->hasParam("file")){
      String name = request->getParam("file")->value();
      debugOutln(name);
      File file = userFS.open("/payloads/" + name);
      if(file){
        String fileContent = file.readString();
        file.close();
        request->send(200, "text/plain", fileContent);
      } else {
        request->send(400, "text/plain", "File not found");
      }
    } else {
      request->send(400, "text/plain", "No file specified");
    }
}

// upload a payload file
void uploadPayloadFileHandler(AsyncWebServerRequest *request) {
    request->send(200);
}
// handle the payload file upload
void payloadFileUploadHandler(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
  debugOutln(logmessage);

  if (!index) {
    logmessage = "Upload Start: " + String(filename);
    // open the file on first call and store the file handle in the request object

    String uploadPath = "";
    if(request->hasParam("path")) uploadPath = request->getParam("path")->value();

    request->_tempFile = userFS.open("/payloads/" + uploadPath + filename, "w");
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

// delete a payload file
void deletePayloadHandler(AsyncWebServerRequest *request) {
    debugOutln("Delete file request:");
    if(request->hasParam("file")){
      String name = request->getParam("file")->value();
      debugOutln("deleting " + name);
      File f = userFS.open("/payloads/" + name);
      if(f.isDirectory()){
        f.close();
        debugOutln("removing dir");
        fileHelper.deleteDir(userFS, ("/payloads/" + name).c_str());
      } else {
        f.close();
        debugOutln("removing file");
        if(!userFS.remove("/payloads/" + name)) debugOutln("Failed");
      }
    }

    request->send(200, "plain/text", "ok");
}

// run a payload
void runPayloadHandler(AsyncWebServerRequest *request) {
    debugOutln("Run payload request:");
    if(request->hasParam("payload")){
      String name = request->getParam("payload")->value();
      payloadToRun = name;
      needToRunPayload = true;
    }

    request->send(200, "plain/text", "ok");
}

// edit a payload
void editPayloadHandler(AsyncWebServerRequest *request) {
    debugOutln("Edit payload request:");
    if(request->hasParam("payload")){
      String name = request->getParam("payload")->value();
      debugOutln("/payloads/" + name);
      editPayload = name;
    }

    request->send(deviceFS, "/web/edit.html", "text/html");
}

// create a new payload
void createPayloadHandler(AsyncWebServerRequest *request) {
    debugOutln("Create payload request:");
    if(request->hasParam("name")){
      String name = request->getParam("name")->value();
      debugOutln("creating " + name);
      File f = userFS.open("/payloads/" + name + "/" + name + ".dd", "w", true);
      if(f){
        f.print("TEST");
        f.close();
        request->send(200, "plain/text", "ok");
      } else {
        request->send(500, "plain/text", "Failed to create payload");
      }
    } else {
      request->send(400, "plain/text", "No name specified");
    }
}

// get the firmware version
void getFirmwareVersionHandler(AsyncWebServerRequest *request) {
    debugOutln("Get firmware version request:");
    String data = "{\n\"version\": \"" + getFirmwareVersion() + "\",\n\"date\": \"" + getCompileDate() + "\"\n}";
    request->send(200, "application/json", data);
}
// get the filesystem version
void getFSVersionHandler(AsyncWebServerRequest *request) {
    debugOutln("Get Filesystem version request:");
    request->send(deviceFS, "/settings/version.json");
}

// upload a zipped payload
void uploadZippedPayloadHandler(AsyncWebServerRequest *request) {
    debugOutln("Upload zipped payload request:");
    if(request->hasParam("path")){
      String name = request->getParam("path")->value();
      debugOutln(name);
    }
  request->send(200);
}
// handle the zipped payload upload
void zippedPayloadUploadHandler(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
  debugOutln(logmessage);

  String uploadPath = "";

  if (!index) {
    logmessage = "Upload Start: " + String(filename);
    // open the file on first call and store the file handle in the request object

    
    if(request->hasParam("path")) uploadPath = request->getParam("path")->value();

    request->_tempFile = userFS.open("/tmp/" + filename, "wb");
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
    debugOutln("Unzipping payload: " + String(filename));
    if(!userFS.mkdir("/payloads/" + uploadPath)){
        debugOutln("Failed to create directory: /payloads/" + uploadPath);
        request->send(500, "text/plain", "Failed to create directory");
        return;
    }
    // Unzip the file
    int rc = unzipAll(&userFS, ("/tmp/" + filename).c_str(), ("/payloads/" + uploadPath).c_str(), true);
    if (rc < 0) {
      debugOut("Unzip failed with error code: ");
      debugOut(rc);
      debugOutln("");
      request->send(500, "text/plain", "Unzip failed");
      return;
    }
    debugOutln("Unzip successful");

    request->redirect("/index.html");
  }
}

// Function to set up the request handlers for the web server
void setRequestHandlers(AsyncWebServer *server){
    server->on("/", HTTP_GET, [] (AsyncWebServerRequest *request){
      request->send(deviceFS, "/web/index.html");
    });

    server->on("/saveSettings"        , HTTP_POST, saveSettingsHandler, NULL, settingsFileUploadHandler);
    server->on("/getSettings"         , HTTP_GET,  getSettingsHandler);
    server->on("/getSettingsInfo"     , HTTP_GET,  getSettingsInfoHandler);
    server->on("/getPayloads"         , HTTP_GET,  getPayloadsHandler);
    server->on("/getFile"             , HTTP_GET,  getPayloadFileHandler);
    server->on("/uploadFile"          , HTTP_POST, uploadPayloadFileHandler, payloadFileUploadHandler);
    server->on("/deleteFile"          , HTTP_GET,  deletePayloadHandler);
    server->on("/runPayload"          , HTTP_GET,  runPayloadHandler);
    server->on("/edit.html"           , HTTP_GET,  editPayloadHandler);
    server->on("/createPayload"       , HTTP_GET,  createPayloadHandler);
    server->on("/getFirmwareVersion"  , HTTP_GET,  getFirmwareVersionHandler);
    server->on("/getFSVersion"        , HTTP_GET,  getFSVersionHandler);
    server->on("/uploadZippedPayload" , HTTP_POST, uploadZippedPayloadHandler, zippedPayloadUploadHandler);
}