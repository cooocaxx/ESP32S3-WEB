#pragma  once
#include <LittleFS.h>
#include <cJSON.h>
#include <vector>
#include <map>
#include "../wifi/wifiConfig.h"
#define FILESYSTEM LittleFS
void Analyze_json(String parse, std::vector<String>& keys,std::map<String, String>& result);
