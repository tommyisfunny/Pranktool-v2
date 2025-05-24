#include <wlan.h>

AsyncWebServer server(80);

void onOTAStart(){
  debugOutln("OTA update started...");
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

    if(password == "") WiFi.softAP(ssid, NULL    , 1, hidden?1:0);
    else               WiFi.softAP(ssid, password, 1, hidden?1:0);

    IPAddress IP = WiFi.softAPIP();
    debugOut("AP IP address: ");
    debugOutln(IP.toString());

    setRequestHandlers(&server);

    server.serveStatic("/", deviceFS, "/web/").setCacheControl("no-cache, no-store, must-revalidate");

    ElegantOTA.onStart(onOTAStart);
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


