/*
  FSWebServer - Example WebServer with FS backend for esp8266/esp32
  Copyright (c) 2015 Hristo Gochkov. All rights reserved.
  This file is part of the WebServer library for Arduino environment.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  upload the contents of the data folder with MkSPIFFS Tool ("ESP32 Sketch Data
  Upload" in Tools menu in Arduino IDE) or you can upload the contents of a
  folder if you CD in that folder and run the following command: for file in `ls
  -A1`; do curl -F "file=@$PWD/$file" esp32fs.local/edit; done

  access the sample web page at http://esp32fs.local
  edit the page by going to http://esp32fs.local/edit
*/
#include "wifi/wifiConfig.h"
#include "info.h"
// #include <NetworkClient.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include "./FileSystem/file_system.h"
#include <Update.h>



// #if FILESYSTEM == FFat
// #include <FFat.h>
// #endif
// #if FILESYSTEM == SPIFFS
// #include <SPIFFS.h>
// #endif

String host = "esp32fs";


WebServer server(80);
// holds the current upload
File fsUploadFile;

// format bytes
String formatBytes(size_t bytes) {
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  } else {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
  }
}




void handleFileUpload() {
  if (server.uri() != "/edit") {
    return;
  }
  HTTPUpload &upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) {
      filename = "/" + filename;
    }
    DBG_OUTPUT_PORT.print("handleFileUpload Name: ");
    DBG_OUTPUT_PORT.println(filename);
    fsUploadFile = FILESYSTEM.open(filename, "w");
    filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    // DBG_OUTPUT_PORT.print("handleFileUpload Data: ");
    // DBG_OUTPUT_PORT.println(upload.currentSize);
    if (fsUploadFile) {
      fsUploadFile.write(upload.buf, upload.currentSize);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile) {
      fsUploadFile.close();
    }
    DBG_OUTPUT_PORT.print("handleFileUpload Size: ");
    DBG_OUTPUT_PORT.println(upload.totalSize);
  }
}





void onServeStaticSubDir(File &dir, String topDir) {
  File file = dir.openNextFile();
  while (file) {
    String path = String(file.path());
    String uri = path.substring(path.indexOf(topDir) + topDir.length(),
                                path.indexOf(".gz"));
    server.serveStatic(uri.c_str(), FILESYSTEM, path.c_str());
    DBG_OUTPUT_PORT.print("uri: ");DBG_OUTPUT_PORT.println(uri);
    DBG_OUTPUT_PORT.print("path: ");
    DBG_OUTPUT_PORT.println(path);
    file = dir.openNextFile();
  }
}

void getStatus() {
  cJSON *rspObject = NULL;
  cJSON *sysObject = NULL;
  cJSON *archObject = NULL;
  cJSON *memObject = NULL;
  cJSON *fsObject = NULL;
  cJSON *apObject = NULL;
  cJSON *staObject = NULL;

  rspObject = cJSON_CreateObject();
  if (rspObject == NULL) {
    goto OUT1;
  }

  sysObject = cJSON_CreateObject();
  if (sysObject == NULL) {
    goto OUT;
  }
  cJSON_AddItemToObject(rspObject, "sys", sysObject);
  cJSON_AddStringToObject(sysObject, "model", "ESP32S3");
  cJSON_AddStringToObject(sysObject, "fw", APP_VERSION);
  cJSON_AddStringToObject(sysObject, "sdk", ESP.getSdkVersion());
  archObject = cJSON_CreateObject();
  if (archObject == NULL) {
    goto OUT;
  }
  cJSON_AddItemToObject(sysObject, "arch", archObject);
  cJSON_AddStringToObject(archObject, "mfr", "Espressif");
  cJSON_AddStringToObject(archObject, "model", ESP.getChipModel());
  cJSON_AddNumberToObject(archObject, "revision", ESP.getChipRevision());
  if (!strncmp(ESP.getChipModel(), "ESP32-S3", strlen("ESP32-S3"))) {
    cJSON_AddStringToObject(archObject, "cpu", "XTensa® dual-core LX7");
  } else if (!strncmp(ESP.getChipModel(), "ESP32-S2", strlen("ESP32-S2"))) {
    cJSON_AddStringToObject(archObject, "cpu", "XTensa® single-core LX7");
  } else if (!strncmp(ESP.getChipModel(), "ESP32-C3", strlen("ESP32-C3"))) {
    cJSON_AddStringToObject(archObject, "cpu", "RISC-V");
  } else if (!strncmp(ESP.getChipModel(), "ESP32", strlen("ESP32"))) {
    cJSON_AddStringToObject(archObject, "cpu", "XTensa® dual-core LX6");
  }
  cJSON_AddNumberToObject(archObject, "freq", ESP.getCpuFreqMHz());

  memObject = cJSON_CreateObject();
  if (memObject == NULL) {
    goto OUT;
  }
  cJSON_AddItemToObject(rspObject, "mem", memObject);
  cJSON_AddNumberToObject(memObject, "total", ESP.getHeapSize());
  cJSON_AddNumberToObject(memObject, "free", ESP.getFreeHeap());

  fsObject = cJSON_CreateObject();
  if (fsObject == NULL) {
    goto OUT;
  }
  cJSON_AddItemToObject(rspObject, "fs", fsObject);
  cJSON_AddNumberToObject(fsObject, "total", FILESYSTEM.totalBytes());
  cJSON_AddNumberToObject(fsObject, "used", FILESYSTEM.usedBytes());
  cJSON_AddNumberToObject(fsObject, "free",
                          FILESYSTEM.totalBytes() - FILESYSTEM.usedBytes());

  apObject = cJSON_CreateObject();
  if (apObject == NULL) {
    goto OUT;
  }
  cJSON_AddItemToObject(rspObject, "ap", apObject);
  cJSON_AddStringToObject(apObject, "ssid", WiFi.softAPSSID().c_str());
  cJSON_AddStringToObject(apObject, "pwd", WiFi.softAPSSID().c_str());
  cJSON_AddNumberToObject(apObject, "num", WiFi.softAPgetStationNum());

  staObject = cJSON_CreateObject();
  if (staObject == NULL) {
    goto OUT;
  }
  cJSON_AddItemToObject(rspObject, "sta", staObject);
  cJSON_AddStringToObject(staObject, "ssid", config_wifi.sta_ssid.c_str());
  cJSON_AddStringToObject(staObject, "pwd", config_wifi.sta_password.c_str());
  cJSON_AddStringToObject(staObject, "status",
                          WiFi.isConnected() ? "connected" : "disconnect");

  server.send(200, "application/json", cJSON_Print(rspObject));
OUT:
  cJSON_Delete(rspObject);
OUT1:
  return;
}


void getWifiMode()
{
  cJSON *Wifi_Mode_Object = cJSON_CreateObject();
  if(Wifi_Mode_Object == NULL) return;
  cJSON_AddStringToObject(Wifi_Mode_Object, "wifi_ap", "true");
  cJSON_AddStringToObject(Wifi_Mode_Object, "wifi_sta", "true");
  server.send(200, "application/json", cJSON_Print(Wifi_Mode_Object));
}

void ConfigNet() {
    
    // // 检查请求 body
    // if (!request->hasArg("plain")) {
    //     request->send(400, "application/json", "{\"error\":\"Missing request body\"}");
    //     return;
    // }

    String content = server.arg("plain");
    cJSON *reqObject = cJSON_Parse(content.c_str());
    if (!reqObject) {
        Serial.println("JSON parse error");
        Serial.print("payload: ");
        Serial.println(content);
        server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
        return;
    }

    // 获取 wifi-sta 对象
    cJSON *wifiObject = cJSON_GetObjectItem(reqObject, "wifi-sta");
    if (!wifiObject) {
        cJSON_Delete(reqObject);
        server.send(400, "application/json", "{\"error\":\"Missing wifi-sta object\"}");
        return;
    }

    // 获取 ssid 和 password
    cJSON *ssidObject = cJSON_GetObjectItem(wifiObject, "ssid");
    cJSON *psdObject = cJSON_GetObjectItem(wifiObject, "password");
    if (!ssidObject || !psdObject || !ssidObject->valuestring || !psdObject->valuestring) {
        cJSON_Delete(reqObject);
        server.send(400, "application/json", "{\"error\":\"Missing ssid or password\"}");
        return;
    }

    bool flag = false;
    String newSsid = ssidObject->valuestring;
    String newPassword = psdObject->valuestring;

    // 检查是否需要更新 Wi-Fi 配置
    if (newSsid != config_wifi.sta_ssid) {
        config_wifi.sta_ssid = newSsid;
        flag = true;
    }
    if (newPassword != config_wifi.sta_password) {
        config_wifi.sta_password = newPassword;
        flag = true;
    }

    // 读取现有 db.json
    String existingContent;
    File configfile = FILESYSTEM.open("/db.json", FILE_READ);
    if (configfile) {
        while (configfile.available()) {
            existingContent += (char)configfile.read();
        }
        configfile.close();
    }

    // 解析现有 db.json
    cJSON *dbObject = existingContent.length() ? cJSON_Parse(existingContent.c_str()) : cJSON_CreateObject();
    if (!dbObject) {
        cJSON_Delete(reqObject);
        server.send(500, "application/json", "{\"error\":\"Failed to parse db.json\"}");
        return;
    }

    // 更新或添加 wifi-sta
    cJSON *dbWifiSta = cJSON_GetObjectItem(dbObject, "wifi-sta");
    if (!dbWifiSta) {
        dbWifiSta = cJSON_CreateObject();
        cJSON_AddItemToObject(dbObject, "wifi-sta", dbWifiSta);
    }
    cJSON_SetValuestring(cJSON_GetObjectItem(dbWifiSta, "ssid") ? cJSON_GetObjectItem(dbWifiSta, "ssid") : cJSON_AddStringToObject(dbWifiSta, "ssid", ""), newSsid.c_str());
    cJSON_SetValuestring(cJSON_GetObjectItem(dbWifiSta, "password") ? cJSON_GetObjectItem(dbWifiSta, "password") : cJSON_AddStringToObject(dbWifiSta, "password", ""), newPassword.c_str());

    // 写入 db.json
    char *updatedJson = cJSON_Print(dbObject);
    configfile = FILESYSTEM.open("/db.json", FILE_WRITE);
    if (!configfile) {
        cJSON_Delete(reqObject);
        cJSON_Delete(dbObject);
        cJSON_free(updatedJson);
        server.send(500, "application/json", "{\"error\":\"Failed to open db.json\"}");
        return;
    }
    configfile.write((const uint8_t *)updatedJson, strlen(updatedJson));
    configfile.close();
    cJSON_free(updatedJson);

    // 清理 cJSON 对象
    cJSON_Delete(reqObject);
    cJSON_Delete(dbObject);

    // 返回响应
    server.send(201, "application/json", "{\"status\":\"success\"}");
    ESP.restart();  
    // 重新连接 Wi-Fi
    // if (flag) {
    //     WiFi.disconnect();
    //     delay(1000);
    //     WiFi.begin(config_wifi.sta_ssid.c_str(), config_wifi.sta_password.c_str());

    // }
}
void setup(void) {
  DBG_OUTPUT_PORT.begin(115200);
  DBG_OUTPUT_PORT.setDebugOutput(true);
  if (FORMAT_FILESYSTEM) {
    FILESYSTEM.format();
  }
  if (FILESYSTEM.begin()) {
    DBG_OUTPUT_PORT.print("FileSystem init sucess\n");

  } // mount with format if needed

  {
    File root = FILESYSTEM.open("/");
    File file = root.openNextFile();
    while (file) {
      String fileName = file.name();
      size_t fileSize = file.size();
      DBG_OUTPUT_PORT.printf("FS File: %s, size: %s\n", fileName.c_str(),
                             formatBytes(fileSize).c_str());
      file = root.openNextFile();
    }
    DBG_OUTPUT_PORT.printf("\n");
  }
  //// 初始化数据结构
  std::map<String, String> wifiInfo;
  std::vector<String> keys = {"ssid", "password"};    
  /// 解析 JSON 中 wifi文件
  Analyze_json("wifi-sta", keys,wifiInfo);
  config_wifi.sta_ssid =  wifiInfo[keys[0]];
  config_wifi.sta_password = wifiInfo[keys[1]];

  Analyze_json("wifi-ap", keys,wifiInfo);
  config_wifi.ap_ssid =  wifiInfo[keys[0]];
  config_wifi.ap_password = wifiInfo[keys[1]];
  ////开始连接 wifi
  wifiSetup();

  MDNS.begin(host);
  DBG_OUTPUT_PORT.print("Open http://");
  DBG_OUTPUT_PORT.print(host);
  DBG_OUTPUT_PORT.println(".local/edit to see the file browser");
  // tag:kaka modify


// 1. 定义所有 API 路由
  server.on("/api/v1/status", HTTP_GET, getStatus);
  server.on("/api/v1/get-wifiMode", HTTP_GET, getWifiMode);
  server.on("/api/v1/config", HTTP_POST, ConfigNet);

  // 2. 定义 OTA 更新路由
  server.on(
      "/api/v1/update", HTTP_POST, 
      []() { // on Success
          server.sendHeader("Connection", "close");
          server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
          delay(500); //【重要】给服务器一点时间发送响应
          ESP.restart(); 
      }, 
      []() { // on Upload
          HTTPUpload &upload = server.upload();
          if (upload.status == UPLOAD_FILE_START) {
              Serial.setDebugOutput(true);
              Serial.printf("Update: %s\n", upload.filename.c_str());
              if (!Update.begin()) {
                  
                  Update.printError(Serial);
              }
          } else if (upload.status == UPLOAD_FILE_WRITE) {
              if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
                  Update.printError(Serial);
              }else {
                // 添加节流逻辑，避免刷屏
                static int last_progress = -1;
                int current_progress = (Update.progress()) / Update.size();

                if (current_progress > last_progress) {
                    Serial.printf("Progress: %d%%\n", current_progress);
                    last_progress = current_progress;
                }

                
              }
          } else if (upload.status == UPLOAD_FILE_END) {
              if (Update.end(true)) {
                  Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
              } else {

                  Update.printError(Serial);
              }
              Serial.setDebugOutput(false);
          } else {
              Serial.printf("Update Failed Unexpectedly: status=%d\n", upload.status);
          }
      }
  );

  // 3. 【关键】配置静态文件服务和 SPA 回退
  // serveStatic 会处理所有对 /www 目录下真实文件的请求。
  // 它也会自动处理对根目录 `/` 的请求，并寻找 index.html。
  server.serveStatic("/", FILESYSTEM, "/www/");

  // 4. 【关键】修改 onNotFound 处理器以支持前端路由
  server.onNotFound([]() {
    // 检查请求的 URI 是否是前端路由（不是 API，也不是一个文件）
    // 对于 WebServer，我们做一个简化处理：如果 serveStatic 没找到文件，
    // 并且请求的 URI 不包含点号（通常文件都包含点号），我们就认为是前端路由。
    if (!server.uri().startsWith("/api") && server.uri().indexOf('.') == -1) {
      // 返回主页，让 Vue Router 处理
      File file = FILESYSTEM.open("/www/index.html", "r");
      if (file) {
        server.streamFile(file, "text/html");
        file.close();
        return; // 确保执行完毕后返回
      }
    }
    
    // 对于其他所有情况（找不到的 API，或找不到的带后缀名的文件），返回 404
    server.send(404, "text/plain", "Not Found");
  });

  // =========================================================================
  // ======================  路由配置修改结束  ===============================
  // =========================================================================

  server.begin();
  DBG_OUTPUT_PORT.println("HTTP server started");

}

void loop(void) {
  server.handleClient();
  delay(2); // allow the cpu to switch to other tasks
}
