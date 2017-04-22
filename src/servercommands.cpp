#include "servercommands.h"
#include "globals.h"
#include "chat.h"
#include "chatcolor.h"
#include "utils.h"

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

        void register_commands() {
            Globals::pCVar->RegisterConCommand(&cvAllowClientCommands);
            Globals::pCVar->RegisterConCommand(&ccColorSay);
            Globals::pCVar->RegisterConCommand(&ccColorSaySpec);
            Globals::pCVar->RegisterConCommand(&ccColorSayT);
            Globals::pCVar->RegisterConCommand(&ccColorSayCt);
        }

    }
}
