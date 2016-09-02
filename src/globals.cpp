#include "globals.h"
#include "utils.h"
#include <iserverplugin.h>

namespace colorsay {
    namespace Globals {
        ICvar *pCVar;
        IVEngineServer *pEngine;
        IPlayerInfoManager *pPlayerInfoManager;
        CGlobalVars *pGlobals;
        IGameEventManager *pGameEventManager;
        int maxPlayers;

        bool SetGlobals(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) {
            bool ok = true;
            //pCVar = (ICvar *) interfaceFactory(CVAR_INTERFACE_VERSION, NULL);
            pCVar = g_pCVar;
            if(!pCVar) {
                ERROR("No ICvar");
                ok = false;
            }
            pEngine = (IVEngineServer *) interfaceFactory(INTERFACEVERSION_VENGINESERVER, NULL);
            if(!pEngine) {
                ERROR("No IVEngineServer");
                ok = false;
            }
            pPlayerInfoManager = (IPlayerInfoManager *) gameServerFactory(INTERFACEVERSION_PLAYERINFOMANAGER, NULL);
            if(!pPlayerInfoManager) {
                ERROR("No IPlayerInfoManager");
                ok = false;
            }
            pGlobals = pPlayerInfoManager->GetGlobalVars();
            if(!pGlobals) {
                ERROR("No CGlobalVars");
                ok = false;
            }
            pGameEventManager = (IGameEventManager *) interfaceFactory(INTERFACEVERSION_GAMEEVENTSMANAGER, NULL);
            if(!pGameEventManager) {
                ERROR("No IGameEventManager");
                ok = false;
            }
            return ok;
        }
    }
}
