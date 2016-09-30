#pragma once

#include <eiface.h>
#include <iplayerinfo.h>
#include <igameevents.h>

#define ENTINDEX(pEdict) static_cast<int>(pEdict - colorsay::Globals::pGlobals->pEdicts)
#define ENTEDICT(index) static_cast<edict_t *>(colorsay::Globals::pGlobals->pEdicts + index)

namespace colorsay {
    namespace Globals {
        extern ICvar *pCVar;
        extern IVEngineServer *pEngine;
        extern IPlayerInfoManager *pPlayerInfoManager;
        extern CGlobalVars *pGlobals;
        extern IGameEventManager *pGameEventManager;
        extern int maxPlayers;

        bool SetGlobals(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory);
    }
}
