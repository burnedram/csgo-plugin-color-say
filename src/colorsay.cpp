#include "colorsay.h"
#include "recipientfilters.h"
#include "globals.h"
#include "utils.h"
#include <cstrike15_usermessages.pb.h>
#include <tier1.h>

#include <string>
#include <sstream>

using namespace std;

ColorSayPlugin g_Plugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(ColorSayPlugin, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, g_Plugin);

void RegisterConCommands() {
    INFO("Registering console commands...");
}

class EventListener : public IGameEventListener {
    public:
        virtual ~EventListener(void) {};
        virtual void FireGameEvent(KeyValues *event) {
            //DEBUG("EVENT: %s", event->GetName());
        }
};
EventListener eventListener;

ColorSayPlugin *ColorSayPlugin::g_pInstance = nullptr;

ColorSayPlugin::ColorSayPlugin() {
    g_pInstance = this;
}
ColorSayPlugin::~ColorSayPlugin() {}

// Initialize the plugin to run
// Return false if there is an error during startup.
bool ColorSayPlugin::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) {
    INFO("Loading...");
    ConnectTier1Libraries(&interfaceFactory, 1);
    if(!colorsay::Globals::SetGlobals(interfaceFactory, gameServerFactory))
        return false;
    colorsay::Globals::pGameEventManager->AddListener(&eventListener, true);

    // Everything seems to be ok, we can run
    RegisterConCommands();
    INFO("Plugin loaded!");
    return true;
}

// Called when the plugin should be shutdown
void ColorSayPlugin::Unload() {
}

// called when a plugins execution is stopped but the plugin is not unloaded
void ColorSayPlugin::Pause() {
}

// called when a plugin should start executing again (sometime after a Pause() call)
void ColorSayPlugin::UnPause() {
}

// Returns string describing current plugin.  e.g., Admin-Mod.  
const char *ColorSayPlugin::GetPluginDescription() {
    return PLUGIN_NAME " v" PLUGIN_VERSION " by burnedram (github.com/burnedram/csgo-plugin-color-say)";
}

// Called any time a new level is started (after GameInit() also on level transitions within a game)
void ColorSayPlugin::LevelInit(char const *pMapName) {
}

// The server is about to activate
void ColorSayPlugin::ServerActivate(edict_t *pEdictList, int edictCount, int clientMax) {
    colorsay::Globals::maxPlayers = clientMax;
}

// The server should run physics/think on all edicts
void ColorSayPlugin::GameFrame(bool simulating) {
}

// Called when a level is shutdown (including changing levels)
void ColorSayPlugin::LevelShutdown() {
}

// Client is going active
void ColorSayPlugin::ClientActive(edict_t *pEntity) {
}

// Client is fully connected (has received initial baseline of entities)
void ColorSayPlugin::ClientFullyConnect(edict_t *pEntity) {
}

// Client is disconnecting from server
void ColorSayPlugin::ClientDisconnect(edict_t *pEntity) {
}

// Client is connected and should be put in the game
void ColorSayPlugin::ClientPutInServer(edict_t *pEntity, char const *playername) {
}

// Sets the client index for the client who typed the command into their console
void ColorSayPlugin::SetCommandClient(int index) {
}

// A player changed one/several replicated cvars (name etc)
void ColorSayPlugin::ClientSettingsChanged(edict_t *pEdict) {
}

// Client is connecting to server (set retVal to false to reject the connection)
//You can specify a rejection message by writing it into reject
PLUGIN_RESULT ColorSayPlugin::ClientConnect(bool *bAllowConnect, edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen) {
    return PLUGIN_CONTINUE;
}

// The client has typed a command at the console
PLUGIN_RESULT ColorSayPlugin::ClientCommand(edict_t *pEdict, const CCommand &args) {
    if(!strcasecmp("colorsay", args.Arg(0))) {
        int entindex = ENTINDEX(pEdict);
        CCSUsrMsg_SayText msg;
        msg.set_chat(true);
        ChatFilter filter(entindex);

        ChatColor::ID color = ChatColor::random();
        ostringstream ss;
        string text;
        ss << "[" << color << PLUGIN_NAME "\01]";

        if(args.ArgC() == 1 || !strcasecmp("help", args.Arg(1))) {
            ss << " Help";
            text = ss.str();
            msg.set_text(text);
            colorsay::Globals::pEngine->SendUserMessage(filter, CS_UM_SayText, msg);
            colorsay::Globals::pEngine->ClientPrintf(pEdict, text.c_str());
            colorsay::Globals::pEngine->ClientPrintf(pEdict, "\n");

            ss.str("");
            ss << "GitHub: github.com/burnedram/csgo-plugin-color-say";
            text = ss.str();
            msg.set_text(text);
            colorsay::Globals::pEngine->SendUserMessage(filter, CS_UM_SayText, msg);
            colorsay::Globals::pEngine->ClientPrintf(pEdict, text.c_str());
            colorsay::Globals::pEngine->ClientPrintf(pEdict, "\n");

            ss.str("");
            ss << "Available commands: version, list";
            text = ss.str();
            msg.set_text(text);
            colorsay::Globals::pEngine->SendUserMessage(filter, CS_UM_SayText, msg);
            colorsay::Globals::pEngine->ClientPrintf(pEdict, text.c_str());
            colorsay::Globals::pEngine->ClientPrintf(pEdict, "\n");

            return PLUGIN_STOP;
        }
        if(!strcasecmp("version", args.Arg(1))) {
            ss << " Plugin version " PLUGIN_VERSION;
            text = ss.str();
            msg.set_text(text);
            colorsay::Globals::pEngine->SendUserMessage(filter, CS_UM_SayText, msg);
            colorsay::Globals::pEngine->ClientPrintf(pEdict, text.c_str());
            colorsay::Globals::pEngine->ClientPrintf(pEdict, "\n");
            return PLUGIN_STOP;
        }
        if(!strcasecmp("list", args.Arg(1))) {
            ostringstream ss2;
            ChatColor::RGB rgb;

            ss << " Available colors";
            text = ss.str();
            msg.set_text(text);
            colorsay::Globals::pEngine->SendUserMessage(filter, CS_UM_SayText, msg);
            colorsay::Globals::pEngine->ClientPrintf(pEdict, text.c_str());
            colorsay::Globals::pEngine->ClientPrintf(pEdict, "\n");

            ss.str("");
            for (color = ChatColor::min; color < ChatColor::max; color++) {
                ss2 << "(" << (int)color << ") " << color << ChatColor::name[color - ChatColor::min] << "\x01, ";
                ss << ss2.str();
                rgb = ChatColor::rgb[color - ChatColor::min];
                ss2 << "r" << (int)rgb.r;
                ss2 << " g" << (int)rgb.g;
                ss2 << " b" << (int)rgb.b;
                colorsay::Globals::pEngine->ClientPrintf(pEdict, ss2.str().c_str());
                colorsay::Globals::pEngine->ClientPrintf(pEdict, "\n");
                ss2.str("");

                if ((color - ChatColor::min) % 2 == 1) {
                    msg.set_text(ss.str());
                    colorsay::Globals::pEngine->SendUserMessage(filter, CS_UM_SayText, msg);
                    ss.str("");
                }
            }
            ss2 << "(" << (int)color << ") " << color << ChatColor::name[color - ChatColor::min];
            ss << ss2.str();
            ss2 << "\x01, ";
            rgb = ChatColor::rgb[color - ChatColor::min];
            ss2 << "r" << (int)rgb.r;
            ss2 << " g" << (int)rgb.g;
            ss2 << " b" << (int)rgb.b;
            colorsay::Globals::pEngine->ClientPrintf(pEdict, ss2.str().c_str());
            colorsay::Globals::pEngine->ClientPrintf(pEdict, "\n");

            msg.set_text(ss.str());
            colorsay::Globals::pEngine->SendUserMessage(filter, CS_UM_SayText, msg);

            return PLUGIN_STOP;
        }

        ss << " Unknown command";
        text = ss.str();
        msg.set_text(text);
        colorsay::Globals::pEngine->SendUserMessage(filter, CS_UM_SayText, msg);
        colorsay::Globals::pEngine->ClientPrintf(pEdict, text.c_str());
        colorsay::Globals::pEngine->ClientPrintf(pEdict, "\n");

        return PLUGIN_STOP;
    }
    return PLUGIN_CONTINUE;
}

// A user has had their network id setup and validated 
PLUGIN_RESULT ColorSayPlugin::NetworkIDValidated(const char *pszUserName, const char *pszNetworkID) {
    return PLUGIN_CONTINUE;
}

// This is called when a query from IServerPluginHelpers::StartQueryCvarValue is finished.
// iCookie is the value returned by IServerPluginHelpers::StartQueryCvarValue.
// Added with version 2 of the interface.
void ColorSayPlugin::OnQueryCvarValueFinished(QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue) {
}

// added with version 3 of the interface.
void ColorSayPlugin::OnEdictAllocated(edict_t *edict) {
}

void ColorSayPlugin::OnEdictFreed(const edict_t *edict) {
}
