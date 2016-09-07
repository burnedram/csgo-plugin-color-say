#pragma once

#include <iserverplugin.h>

class ColorSayPlugin : public IServerPluginCallbacks {
    public:
        static ColorSayPlugin  *g_pInstance;

                                ColorSayPlugin();
        virtual                 ~ColorSayPlugin();

        // IServerPluginCallbacks methods
        virtual bool            Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory );
        virtual void            Unload();
        virtual void            Pause();
        virtual void            UnPause();
        virtual const char     *GetPluginDescription();      
        virtual void            LevelInit(char const *pszMapName);
        virtual void            ServerActivate(edict_t *pEdictList, int edictCount, int clientMax);
        virtual void            GameFrame(bool simulating);
        virtual void            LevelShutdown();
        virtual void            ClientActive(edict_t *pEntity);
        virtual void            ClientFullyConnect(edict_t *pEntity);
        virtual void            ClientDisconnect(edict_t *pEntity);
        virtual void            ClientPutInServer(edict_t *pEntity, char const *pszPlayername);
        virtual void            SetCommandClient(int index);
        virtual void            ClientSettingsChanged(edict_t *pEdict);
        virtual PLUGIN_RESULT   ClientConnect(bool *bAllowConnect, edict_t *pEntity, const char *pszName, const char *pszAddress, char *pszReject, int maxrejectlen);
        virtual PLUGIN_RESULT   ClientCommand(edict_t *pEntity, const CCommand &args);
        virtual PLUGIN_RESULT   NetworkIDValidated(const char *pszUserName, const char *pszNetworkID);
        virtual void            OnQueryCvarValueFinished(QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pszCvarName, const char *pszCvarValue);
        virtual void            OnEdictAllocated(edict_t *pEdict);
        virtual void            OnEdictFreed(const edict_t *pEdict);  
};

extern void RegisterConCommands();
