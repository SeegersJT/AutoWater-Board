#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <FS.h>
#include <SPIFFS.h>
#include <map>
#include <Arduino.h>
#include "display_wrapper.h"

class ConfigManager {
public:
    void initialConfig();
    void readFromINI();
    std::map<String, int> config_map;
    int& operator[](const String& key);
};

#endif // CONFIG_MANAGER_H
