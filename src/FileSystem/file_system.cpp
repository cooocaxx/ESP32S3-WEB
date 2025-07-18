#include "file_system.h"
#include "../info.h"


/**
 * @brief 解析 db.json
 * 
 * @param parse 
 */
void Analyze_json(String parse, std::vector<String>& keys, std::map<String, String>& result)
{

    DBG_OUTPUT_PORT.println("appLoadDateBase");

    File dbfile = FILESYSTEM.open("/db.json", "r");
    if (!dbfile)
    {
        DBG_OUTPUT_PORT.println("Error opening file.");
        return;
    }

    char *buffer = (char *)malloc(dbfile.size());

    while (dbfile.available())
    {
        dbfile.readBytes(buffer, dbfile.size());
    }

    DBG_OUTPUT_PORT.println(buffer);

    cJSON *rootObject = cJSON_ParseWithLength(buffer, dbfile.size());
    if (rootObject == NULL)
    {
        dbfile.close();
        return;
    }

    cJSON *wifiObject = cJSON_GetObjectItem(rootObject, parse.c_str());
    cJSON *ssidObject = cJSON_GetObjectItem(wifiObject, "ssid");
    cJSON *psdObject = cJSON_GetObjectItem(wifiObject, "password");

    // result['']= String(ssidObject->valuestring);
    // args[1] = String(psdObject->valuestring);

    for (const auto& key : keys) {
        cJSON *item = cJSON_GetObjectItem(wifiObject, key.c_str());
        if (item && item->valuestring) {
            result[key] = String(item->valuestring);
            DBG_OUTPUT_PORT.printf("%s:%s \n",key.c_str(),item->valuestring);
        }else {
            DBG_OUTPUT_PORT.printf("%s: 键未找到或值为空\n", key.c_str());
        }
    }
    // DBG_OUTPUT_PORT.print("ssid: ");
    // DBG_OUTPUT_PORT.println(config_wifi_sta->ssid);
    // DBG_OUTPUT_PORT.print("password: ");
    // DBG_OUTPUT_PORT.println(config_wifi_sta->password);

    free(buffer);
    cJSON_Delete(rootObject);
    dbfile.close();

    DBG_OUTPUT_PORT.println("appLoadDateBase done");
}