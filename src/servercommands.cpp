#include "servercommands.h"
#include "globals.h"
#include "chat.h"
#include "chatcolor.h"
#include "utils.h"

#include <unordered_set>
using namespace std;

#define UNASSIGNED 0
#define SPECTATOR 1
#define TERRORIST 2
#define COUNTERTERRORIST 3

namespace colorsay {
    namespace servercommands {

        ConVar cvAllowClientCommands("colorsay_allow_client", "0",
                FCVAR_SERVER_CAN_EXECUTE,
                "Allow clients to use the \"colorsay\" command",
                true, 0, true, 1);

        static void cc_colorsay(const CCommand &args) {
            string parsed(args.ArgS());
            if (!chatcolor::parse_colors(parsed))
                INFO("Message contains bad tags");
            chat::say_all(parsed);
        }
        ConCommand ccColorSay("colorsay", cc_colorsay,
                "Display colored player message",
                FCVAR_SERVER_CAN_EXECUTE);

        static void cc_colorsay_team(int team, const CCommand &args) {
            string parsed(args.ArgS());
            if (!chatcolor::parse_colors(parsed))
                INFO("Message contains bad tags");
            chat::say_team(team, parsed);
        }

        static void cc_colorsay_spec(const CCommand &args) {
            cc_colorsay_team(SPECTATOR, args);
        }
        ConCommand ccColorSaySpec("colorsay_spec", cc_colorsay_spec,
                "Display colored player message to Spectators",
                FCVAR_SERVER_CAN_EXECUTE);

        static void cc_colorsay_t(const CCommand &args) {
            cc_colorsay_team(TERRORIST, args);
        }
        ConCommand ccColorSayT("colorsay_t", cc_colorsay_t,
                "Display colored player message to Terrorists",
                FCVAR_SERVER_CAN_EXECUTE);

        static void cc_colorsay_ct(const CCommand &args) {
            cc_colorsay_team(COUNTERTERRORIST, args);
        }
        ConCommand ccColorSayCt("colorsay_ct", cc_colorsay_ct,
                "Display colored player message to CTs",
                FCVAR_SERVER_CAN_EXECUTE);

        static void cc_colorsay_id(const CCommand &args) {
            if (args.ArgC() < 3) {
                ConMsg("usage: colorsay_id <comma separated ids> <message>\n");
                return;
            }

            unordered_set<string> netids;
            netids.reserve(Globals::maxPlayers);
            string netstring = args.Arg(1);
            size_t pos = 0, csv = netstring.find(',');
            while (true) {
                size_t len = csv == string::npos ? string::npos : csv - pos;
                string netid = netstring.substr(pos, len);
                if (!netid.empty()) {
                    netids.insert(netid);
                }
                if (csv == string::npos)
                    break;
                pos = csv + 1;
                csv = netstring.find(',', pos);
            }

            vector<int> indices;
            indices.reserve(Globals::maxPlayers);
            for (int index = 1; index <= Globals::maxPlayers; index++) {
                edict_t *pEdict = ENTEDICT(index);
                if(Globals::pEngine->GetPlayerUserId(pEdict) == -1)
                    continue;
                IPlayerInfo *pInfo = Globals::pPlayerInfoManager->GetPlayerInfo(pEdict);
                const char *pNetId = pInfo->GetNetworkIDString();
                if (pNetId && netids.find(pNetId) != netids.end())
                    indices.push_back(index);
            }
            if (!indices.empty()) {
                string stl_args(args.ArgS() + netstring.length());
                size_t trim = stl_args.find_first_of(" \t");
                if(trim != string::npos)
                    stl_args = stl_args.substr(trim);
                trim = stl_args.find_first_not_of(" \t");
                if(trim != string::npos)
                    stl_args = stl_args.substr(trim);

                if (!chatcolor::parse_colors(stl_args))
                    ConMsg("Message contains bad tags");
                chat::say(indices, stl_args);
            }
        }
        ConCommand ccColorSayId("colorsay_id", cc_colorsay_id,
                "Display colored player message to specified id(s)",
                FCVAR_SERVER_CAN_EXECUTE);

        static void cc_colorsay_version(const CCommand &args) {
            ConMsg(PLUGIN_VERSION "\n");
        }
        ConCommand ccColorSayVersion("colorsay_version", cc_colorsay_version,
                "Show ColorSay version",
                FCVAR_SERVER_CAN_EXECUTE);

        void register_commands() {
            Globals::pCVar->RegisterConCommand(&cvAllowClientCommands);
            Globals::pCVar->RegisterConCommand(&ccColorSay);
            Globals::pCVar->RegisterConCommand(&ccColorSaySpec);
            Globals::pCVar->RegisterConCommand(&ccColorSayT);
            Globals::pCVar->RegisterConCommand(&ccColorSayCt);
            Globals::pCVar->RegisterConCommand(&ccColorSayId);
            Globals::pCVar->RegisterConCommand(&ccColorSayVersion);
        }

    }
}
