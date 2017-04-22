#pragma once
#include <convar.h>

namespace colorsay {
    namespace servercommands {
        extern ConVar cvAllowClientCommands;
        void register_commands();
    }
}
