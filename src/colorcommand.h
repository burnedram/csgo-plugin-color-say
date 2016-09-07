#pragma once
#include <iserverplugin.h>
#include <string>
#include <vector>

namespace colorsay {
    class ColorCommand {
    public:
        virtual ~ColorCommand() {};
        virtual const std::string get_name() const = 0;
        virtual const std::string get_description() const = 0;
        virtual const std::string get_usage() const = 0;
        virtual const std::string get_help() const = 0;
        virtual PLUGIN_RESULT invoke(edict_t *pEdict, const std::string &args, const std::vector<std::string> &argv) const = 0;
    };
}
