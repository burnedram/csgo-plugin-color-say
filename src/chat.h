#pragma once

#include <eiface.h>
#include <irecipientfilter.h>
#include <string>
#include <vector>

namespace colorsay {
    namespace chat {
        void say(edict_t *pEdict, const std::string &str);
        void say(const std::vector<edict_t *> pEdicts, const std::string &str);
        void say(const IRecipientFilter &filter, const std::string &str);
        void parse_colors(std::string &str);
        void strip_colors(std::string &str);
    }
}
