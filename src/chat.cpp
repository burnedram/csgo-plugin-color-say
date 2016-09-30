#include "chat.h"
#include "recipientfilters.h"
#include "globals.h"
#include "utils.h"

#include <eiface.h>
#include <cstrike15_usermessages.pb.h>

using namespace std;

namespace colorsay {
    namespace chat {

        void say_all(const string &str) {
            vector<int> indices;
            indices.reserve(Globals::maxPlayers);
            for(int index = 1; index <= Globals::maxPlayers; index++) {
                edict_t *pEdict = ENTEDICT(index);
                if(Globals::pEngine->GetPlayerUserId(pEdict) == -1)
                    continue;
                indices.push_back(index);
            }
            say(indices, str);
        }

        void say(edict_t *pEdict, const string &str) {
            ChatFilter filter(ENTINDEX(pEdict));
            say(filter, str);
        }
        
        void say(const vector<edict_t *> pEdicts, const string &str) {
            vector<int> indices;
            indices.reserve(pEdicts.size());
            for(auto &pEdict : pEdicts)
                indices.push_back(ENTINDEX(pEdict));
            say(indices, str);
        }

        void say(const vector<int> indices, const string &str) {
            RecipientFilter filter(indices.size());
            for(auto &index : indices)
                filter.AddRecipient(index);
            filter.MakeReliable();
            say(filter, str);
        }

        void say(const IRecipientFilter &filter, const string &str) {
            CCSUsrMsg_SayText msg;
            msg.set_chat(true);
            msg.set_text(str);
            Globals::pEngine->SendUserMessage(const_cast<IRecipientFilter &>(filter), CS_UM_SayText, msg);
        }

    }
}
