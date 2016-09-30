#include "console.h"
#include "globals.h"
#include "chatcolors.h"

using namespace std;

namespace colorsay {
    namespace console {
        void print(edict_t *pEdict, const string &msg) {
            Globals::pEngine->ClientPrintf(pEdict, msg.c_str());
        }

        void println(edict_t *pEdict) {
            Globals::pEngine->ClientPrintf(pEdict, "\n");
        }

        void println(edict_t *pEdict, const string &msg) {
            print(pEdict, msg);
            Globals::pEngine->ClientPrintf(pEdict, "\n");
        }
    }
}
