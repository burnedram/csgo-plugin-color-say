#pragma once
#include "constants.h"
#include <iserverplugin.h>

#define LOG(level, format, ...) ConMsg(PLUGIN_NAME ": [" level "] " format"\n", ##__VA_ARGS__)
#define INFO( format, ...) LOG("INFO",  format, ##__VA_ARGS__)
#define DEBUG(format, ...) LOG("DEBUG", format, ##__VA_ARGS__)
#define WARN( format, ...) LOG("WARN",  format, ##__VA_ARGS__)
#define ERROR(format, ...) LOG("ERROR", format, ##__VA_ARGS__)
