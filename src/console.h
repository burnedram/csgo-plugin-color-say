#pragma once
#include <eiface.h>
#include <string>

namespace colorsay {
    namespace console {
        void print(edict_t *pEdict, const std::string &msg, bool stripcolors = true);
        void println(edict_t *pEdict, const std::string &msg, bool stripcolors = true);
    }
}
