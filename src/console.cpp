#include "console.h"
#include "globals.h"
#include "chatcolors.h"

using namespace std;

namespace colorsay {
    namespace console {
        void print(edict_t *pEdict, const string &msg, bool stripcolors) {
            if(stripcolors) {
                string stripped(msg);
                chatcolor::strip_colors(stripped);
                Globals::pEngine->ClientPrintf(pEdict, stripped.c_str());
            } else
                Globals::pEngine->ClientPrintf(pEdict, msg.c_str());
        }

        void println(edict_t *pEdict, const string &msg, bool stripcolors) {
            print(pEdict, msg, stripcolors);
            Globals::pEngine->ClientPrintf(pEdict, "\n");
        }
    }
}
