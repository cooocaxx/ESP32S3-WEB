#include "wifiConfig.h"
#include <cJSON.h>
#include "../info.h"

long long firsttime = 0;
long long latetime = 1000*10; // 10s
config_wifi_t config_wifi = {};

void wifiSetup()
{
    // DBG_OUTPUT_PORT.println("wifiSetup");
    // uint8_t mac[6] = {0};
    // char ssidStr[32] = {0};
  firsttime = millis();
  DBG_OUTPUT_PORT.printf("get wifi config:%s,%s\n", config_wifi.sta_ssid.c_str(),config_wifi.sta_password.c_str());
  DBG_OUTPUT_PORT.printf("Connecting to %s\n", config_wifi.sta_ssid);
  if (String(WiFi.SSID()) != String(config_wifi.sta_ssid)) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(config_wifi.sta_ssid.c_str(), config_wifi.sta_password.c_str());
  }

  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - firsttime > latetime) {
      DBG_OUTPUT_PORT.println("WiFi: Connection timed out");
      ap_mode_init();
      return;
    }else
    {
      delay(500);
      DBG_OUTPUT_PORT.print(".");
    }
  }
  DBG_OUTPUT_PORT.println("");
  DBG_OUTPUT_PORT.print("Connected! IP address: ");
  DBG_OUTPUT_PORT.println(WiFi.localIP());



    // WiFi.disconnect(true);
    // delay(1000);

    // Serial.println("WiFi: Set mode to WIFI_AP_STA");
    // WiFi.mode(WIFI_AP_STA);
    // // WiFi.onEvent(onWiFiEvent);

    // WiFi.begin(config_wifi.ssid.c_str(), config_wifi.password.c_str());

    // // Wait for WiFi to connect to AP
    // Serial.println("Waiting for WiFi");

    // WiFi.softAPmacAddress(mac);
    // sprintf(ssidStr, "ESP-%02X%02X%02X", mac[3], mac[4], mac[5]);
    // if (WiFi.softAP(ssidStr) != true)
    // {
    //     Serial.println("WiFi: failed to create softAP");
    //     return;
    // }

    // config_wifi.apSSID = String(ssidStr);
    // Serial.println("WiFi: softAP has been established");
    // Serial.printf("WiFi: please connect to the %s\r\n", config_wifi.apSSID);
}

void ap_mode_init()
{
    uint8_t mac[6] = {0};
    // char ssidStr[32] = {0};

    WiFi.disconnect(true);
    delay(1000);

    Serial.println("WiFi: Set mode to WIFI_AP_STA");
    WiFi.mode(WIFI_AP_STA);
    // WiFi.onEvent(onWiFiEvent);

    // WiFi.begin(ssid.c_str(), password.c_str());

    // Wait for WiFi to connect to AP
    Serial.println("Waiting for WiFi");

    WiFi.softAPmacAddress(mac);
    //sprintf(ssidStr, "ESP-%02X%02X%02X", mac[3], mac[4], mac[5]);
    if (WiFi.softAP(config_wifi.ap_ssid.c_str()) != true)
    {
      
        Serial.println("WiFi: failed to create softAP");
        return;
    }

    // // apSSID = String(ssidStr);
    // Serial.println("WiFi: softAP has been established");
    // Serial.printf("WiFi: please connect to the %s\r\n", apSSID);
}

void ap_mode_deinit()
{
  
}