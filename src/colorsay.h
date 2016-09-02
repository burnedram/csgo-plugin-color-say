#pragma once

#include <iserverplugin.h>

#include <array>
#include <ostream>

namespace ChatColor {
    enum ID : char {
        WHITE = 1,              RED = 2,                LAVENDER = 3,           GREEN = 4, 
        PALE_GREEN = 5,         GREEN_YELLOW = 6,       CORAL_RED = 7,          LIGHT_GREY = 8,         
        LIGHT_GOLDENROD = 9,    ECHO_BLUE = 10,         PICTON_BLUE = 11,       ROYAL_BLUE = 12,        
        PURPLE = 13,            NEON_PURPLE = 14,       VERMILION = 15,         ORANGE_GOLD = 16
    };

    ID &operator++(ID &id) {
        id = static_cast<ID>(id + 1);
        return id;
    }
    ID operator++(ID &id, int) {
        ID oldVal = id;
        ++id;
        return oldVal;
    }

    std::ostream &operator<<(std::ostream &out, const ID &id) {
        out << static_cast<char>(id);
        return out;
    }

    const ID min = static_cast<ID>(1), max = static_cast<ID>(16);
    const size_t count = max - min + 1;
    const std::array<const char *, count> name = {
        "White",                "Red",                  "Lavender",             "Green",
        "Pale green",           "Green yellow",         "Coral red",            "Light grey",
        "Light Goldenrod",      "Echo blue",            "Picton blue",          "Royal blue",
        "Purple",               "Neon purple",          "Vermilion",            "Orange gold"
    };

    struct RGB {
        uint8_t r, g, b;
    };
    const std::array<const RGB, count> rgb = {
        255, 255, 255,          238,   2,   2,          185, 129, 239,           64, 254,  64,
        190, 254, 143,          161, 254,  71,          254,  64,  64,          196, 201, 207,
        236, 227, 122,          174, 193, 215,           94, 151, 216,           75, 105, 254,
        136,  71, 254,          210,  44, 229,          234,  75,  75,          225, 172,  57
    };

    inline ID random() {
        return static_cast<ID>((rand() % count) + min);
    }
}

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
