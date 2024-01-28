/*
  This software is released under the MIT License.
  https://opensource.org/licenses/MIT

  AutoConnectUpload uploads files from the HTTP client to SPIFFS and
  lets a user to delete or play (not implemented) them.
  with its file name. To run this example successfully, you need to
  initialize the SPIFFS by the PlatfromIO "Upload Filesystem Image" command.
  */


#if defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>
#define FORMAT_ON_FAIL
using WiFiWebServer = ESP8266WebServer;
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#include <WebServer.h>
 
#include <HTTPClient.h>
#include <ESPmDNS.h>
#define FORMAT_ON_FAIL  true
using WiFiWebServer = WebServer;
#endif

#include <AutoConnect.h>
#include <PageBuilder.h>

#ifdef AUTOCONNECT_USE_LITTLEFS
#include <LittleFS.h>
#if defined(ARDUINO_ARCH_ESP8266)
FS& FlashFS = LittleFS;
#elif defined(ARDUINO_ARCH_ESP32)
fs::LittleFSFS& FlashFS = LittleFS;
#endif
#else
#include <FS.h>
#include <SPIFFS.h>
fs::SPIFFSFS& FlashFS = SPIFFS;
#endif

// Include the class to handle the upload process
#include "AutoConnectUploadFS.h"
AutoConnectUploadFS uploader;

// Include the HTML pages coded as PROGMEM
#include "HTMLpages.h"

WebServer server;
AutoConnect portal(server);

// Declare the AutoConnectAux custom web page handlers.
AutoConnectAux auxBrowse;
AutoConnectAux auxPlay;
AutoConnectAux auxDelete;

/* 
Append all the filenames found in the SPIFFS root ("/") to the radio element 
as RadioButtons
*/
void ListAllFiles(AutoConnectRadio * radio){
  Serial.printf("SPIFFS total size: %d bytes. Used %d bytes.\r\n",FlashFS.totalBytes(), FlashFS.usedBytes());
  File root = SPIFFS.open("/");
 
  File file = root.openNextFile();
 
  while(file){
       Serial.printf("FILE: %s  size: %d bytes\r\n",file.name(),file.size());
      radio->add(String(file.name()) + " " + String(file.size()) + " bytes");
      file.close();
      file = root.openNextFile();
  }
  
}

/**
 * Post uploading, AutoConnectFile's built-in upload handler reads the
 * file saved in SPIFFS and displays the file contents on /upload custom
 * web page. However, only files with mime type uploaded as text are
 * displayed. A custom web page handler is called after upload.
 * @param  aux  AutoConnectAux(/browse)
 * @param  args PageArgument
 * @return Uploaded text content
 */
String postUpload(AutoConnectAux& aux, PageArgument& args) {
  String  content;
  
  AutoConnectRadio& radio = aux["radio"].as<AutoConnectRadio>();
  radio.empty();
  ListAllFiles(&radio);
  return String();
}

/*
    handler to play a file
*/
String postAuxPlay(AutoConnectAux& aux, PageArgument& args) {
  
  AutoConnectElement * rr = auxBrowse.getElement("radio");
  if( rr == nullptr )
    return String();
  
  AutoConnectRadio&  radio = auxBrowse["radio"].as<AutoConnectRadio>();
  Serial.printf("radio checked check\r\n");
  Serial.printf("radio checked=%d\r\n", radio.checked);
  if(radio.checked>0){ // 0 means no radio button pushed
    String filename = radio[radio.checked-1];
    filename = filename.substring(0, filename.indexOf(" "));
    Serial.printf("file to be played:filename=%s\r\n", filename.c_str());
  }
  aux.redirect("/browse");
  return String();
}

/*
    handler to delete a file
*/
String postAuxDelete(AutoConnectAux& aux, PageArgument& args) {
  
  AutoConnectElement * rr = auxBrowse.getElement("radio");
  if( rr == nullptr )
    return String();
  
  AutoConnectRadio&  radio = auxBrowse["radio"].as<AutoConnectRadio>();
  Serial.printf("radio checked check\r\n");
  Serial.printf("radio checked=%d\r\n", radio.checked);
  if(radio.checked>0){ // 0 means no radio button pushed
    String filename = radio[radio.checked-1];
    filename = filename.substring(0, filename.indexOf(" "));
    Serial.printf("file to be deleted:%s\r\n", filename.c_str());
    SPIFFS.remove(filename);
  }
  aux.redirect("/browse");
  return String();
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();

  // Start the filesystem
  FlashFS.begin(FORMAT_ON_FAIL);

  // Attach the custom web pages
  portal.load(PAGE_ROOT);
  auxBrowse.load(PAGE_BROWSE);
  auxBrowse.on(postUpload);
  auxBrowse.onUpload(uploader);
  auxPlay.load(PAGE_PLAY);
  auxPlay.on(postAuxPlay);    
  auxDelete.load(PAGE_DELETE);
  auxDelete.on(postAuxDelete);    
  
  portal.join({ auxBrowse, auxPlay, auxDelete });
  
  if(portal.begin()){
    Serial.printf("Started IP: %s\r\n", WiFi.localIP().toString().c_str());
  }
}

void loop() {
  portal.handleClient();
}
