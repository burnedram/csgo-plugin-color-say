#pragma once
#include "colorcommand.h"
#include <string>
#include <vector>

namespace colorsay {
    namespace colorcommands {
        void register_commands();
        bool exists(const std::string &name);
        PLUGIN_RESULT invoke(edict_t *pEdict, const std::string &name, const std::string &args, const std::vector<std::string> &argv);
    }
}
