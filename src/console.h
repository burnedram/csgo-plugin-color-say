#pragma once
#include <eiface.h>
#include <string>

namespace colorsay {
    namespace console {
        void print(edict_t *pEdict, const std::string &msg);
        void println(edict_t *pEdict);
        void println(edict_t *pEdict, const std::string &msg);
    }
}
