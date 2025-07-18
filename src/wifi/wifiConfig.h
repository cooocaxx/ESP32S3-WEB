#pragma once
#include <WiFi.h>

typedef struct
{
    String sta_ssid;
    String sta_password;
    String ap_ssid;
    String ap_password;
    String apSSID;
} config_wifi_t; 

extern config_wifi_t config_wifi;
void ap_mode_init();
void wifiSetup();