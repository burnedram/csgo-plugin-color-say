#include "colorsay.h"
#include "recipientfilters.h"
#include "globals.h"
#include "utils.h"
#include "colorcommands.h"
#include "console.h"
#include <cstrike15_usermessages.pb.h>
#include <tier1.h>

#include <string>
#include <sstream>

using namespace std;

ColorSayPlugin g_pPlugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(ColorSayPlugin, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, g_pPlugin);


ColorSayPlugin::ColorSayPlugin() {}
ColorSayPlugin::~ColorSayPlugin() {}

// Initialize the plugin to run
// Return false if there is an error during startup.
bool ColorSayPlugin::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) {
    INFO("Loading...");
    ConnectTier1Libraries(&interfaceFactory, 1);
    if(!colorsay::Globals::SetGlobals(interfaceFactory, gameServerFactory)) {
        INFO("Plugin NOT loaded!");
        return false;
    }

    INFO("Registering client console commands...");
    colorsay::colorcommands::register_commands();

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
    if(strcasecmp("colorsay", args.Arg(0)))
        return PLUGIN_CONTINUE;
    if(args.ArgC() == 1) {
        ostringstream ss;
        ss << "Usage: colorsay <command>\n";
        colorsay::console::println(pEdict, ss.str().c_str());
        return PLUGIN_STOP;
    }
    if(!colorsay::colorcommands::exists(args.Arg(1))) {
        ostringstream ss;
        ss << "Unknown command \"" << args.Arg(1) << "\"\n";
        colorsay::console::println(pEdict, ss.str().c_str());
        return PLUGIN_STOP;
    }

    vector<string> stl_argv;
    stl_argv.reserve(args.ArgC() - 1);
    for(int i = 1; i < args.ArgC(); i++)
        stl_argv.push_back(args.Arg(i));

    string stl_args(args.ArgS() + strlen(args.Arg(1)));
    size_t trim = stl_args.find_first_not_of(" \t");
    if(trim != string::npos)
        stl_args = stl_args.substr(trim);

    return colorsay::colorcommands::invoke(pEdict, stl_argv.front(), stl_args, stl_argv);
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
