#include "chat.h"
#include "recipientfilters.h"
#include "globals.h"
#include "utils.h"

#include <eiface.h>
#include <cstrike15_usermessages.pb.h>

using namespace std;

namespace colorsay {
    namespace chat {

        void say(edict_t *pEdict, const string &str) {
            ChatFilter filter(ENTINDEX(pEdict));
            say(filter, str);
        }

        void say(const vector<edict_t *> pEdicts, const string &str) {
            RecipientFilter filter(pEdicts.size());
            for(auto &pEdict : pEdicts) {
                filter.AddRecipient(ENTINDEX(pEdict));
            }
            filter.MakeReliable();
            say(filter, str);
        }

        void say(const IRecipientFilter &filter, const string &str) {
            CCSUsrMsg_SayText msg;
            msg.set_chat(true);
            msg.set_text(str);
            Globals::pEngine->SendUserMessage(const_cast<IRecipientFilter &>(filter), CS_UM_SayText, msg);
        }

        /*
         * Parses color tags to their char value, i.e:
         * This text is{#red} red -> This text is\x02 red
         */
        void parce_colors(string &str) {
            //TODO
        }

        /*
         * Removes color tags, i.e:
         * This text is{#red} red -> This text is red
         */
        void strip_colors(string &str) {
            //TODO
        }

    }
}
