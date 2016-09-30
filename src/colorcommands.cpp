#include "colorcommands.h"
#include "globals.h"
#include "constants.h"
#include "chatcolors.h"
#include "chat.h"
#include "console.h"
#include <unordered_map>
#include <sstream>
#include <algorithm>

using namespace std;

namespace colorsay {

    namespace colorcommands {

        static unordered_map<string, ColorCommand *> _commands;

        bool exists(const string &name) {
            string lowername = name;
            std::transform(lowername.begin(), lowername.end(), lowername.begin(), ::tolower);
            return _commands.count(lowername);
        }

        PLUGIN_RESULT invoke(edict_t *pEdict, const string &name, const string &args, const vector<string> &argv) {
            string lowername(name);
            std::transform(lowername.begin(), lowername.end(), lowername.begin(), ::tolower);
            return _commands.at(lowername)->invoke(pEdict, args, argv);
        }

    }

    class HelpCommand : public ColorCommand {
    public:
        virtual const string get_name() const {
            return "help";
        }

        virtual const string get_description() const {
            return "Prints a list of available commands, or more specific help on a command";
        }

        virtual const string get_usage() const {
            return "help <command>";
        }

        virtual const string get_help() const {
            return "Prints a list of available commands, or more specific help on a command";
        }

        virtual PLUGIN_RESULT invoke(edict_t *pEdict, const string &args, const vector<string> &argv) const {
            ostringstream ss;
            if (argv.size() == 2) {
                const string &name = argv[1];
                if(!colorcommands::exists(name)) {
                    ss << "Unknown command \"" << name << "\"\n";
                    console::println(pEdict, ss.str());
                } else {
                    const ColorCommand *cmd = colorcommands::_commands.at(name);
                    ss << "Usage: " << cmd->get_usage() << "\n\n" << cmd->get_help() << "\n";
                    console::println(pEdict, ss.str());
                }
            } else if (argv.size() == 1) {
                console::println(pEdict, "GitHub: github.com/burnedram/csgo-plugin-color-say\nAvailable commands:\n");
                for (auto &pair : colorcommands::_commands) {
                    auto &cc = pair.second;
                    console::print(pEdict, cc->get_usage());
                    console::print(pEdict, "\n\t");
                    console::println(pEdict, cc->get_description());
                }
            } else {
                ss << "Usage: " << get_usage() << "\n";
                console::println(pEdict, ss.str().c_str());
            }
            return PLUGIN_STOP;
        }
    };

    class VersionCommand : public ColorCommand {
    public:
        virtual const string get_name() const {
            return "version";
        }

        virtual const string get_description() const {
            return "Prints the version of this plugin";
        }

        virtual const string get_usage() const {
            return "version";
        }

        virtual const string get_help() const {
            return "Prints the version of this plugin";
        }

        virtual PLUGIN_RESULT invoke(edict_t *pEdict, const string &args, const vector<string> &argv) const {
            console::println(pEdict, "Plugin version " PLUGIN_VERSION);
            ostringstream ss;
            ss << "[" << chatcolor::random() << PLUGIN_NAME << chatcolor::ID::WHITE << "] Plugin version " PLUGIN_VERSION;
            string str = ss.str();
            chatcolor::parse_colors(str);
            chat::say(pEdict, str);
            return PLUGIN_STOP;
        }
    };

    class AvailableColorsCommand : public ColorCommand {
    public:
        virtual const string get_name() const {
            return "list";
        }

        virtual const string get_description() const {
            return "Lists all available colors";
        }

        virtual const string get_usage() const {
            return "list";
        }

        virtual const string get_help() const {
            return "Lists all available colors";
        }

        virtual PLUGIN_RESULT invoke(edict_t *pEdict, const string &args, const vector<string> &argv) const {
            ostringstream ss;
            ostringstream ss2;
            string chat_text, console_text;
            chatcolor::RGB rgb;
            chatcolor::ID color;

            ss << "[" << chatcolor::random() << PLUGIN_NAME << "] Available colors";
            chat_text = ss.str();
            chatcolor::parse_colors(chat_text);
            chat::say(pEdict, chat_text);
            console::println(pEdict, "Available colors");

            ss.str("");
            for (color = chatcolor::min; color <= chatcolor::max; color++) {
                ss2 << "(" << (int)color << ") {#" << (int)color << "}" << chatcolor::name(color);
                if(color < chatcolor::max)
                    ss2 << "{#1}, ";
                ss << ss2.str();
                if(color == chatcolor::max)
                    ss2 << ", ";
                rgb = chatcolor::rgb(color);
                ss2 << "r" << (int)rgb.r;
                ss2 << " g" << (int)rgb.g;
                ss2 << " b" << (int)rgb.b;
                console_text = ss2.str();
                chatcolor::parse_colors(console_text, true);
                console::println(pEdict, console_text);
                ss2.str("");

                if ((color - chatcolor::min) % 2 == 1 || color == chatcolor::max) {
                    chat_text = ss.str();
                    chatcolor::parse_colors(chat_text);
                    chat::say(pEdict, chat_text);
                    ss.str("");
                }
            }

            return PLUGIN_STOP;
        }
    };

    class EchoCommand : public ColorCommand {
    public:
        virtual const string get_name() const {
            return "echo";
        }

        virtual const string get_description() const {
            return "Prints colored text for you (and only you)";
        }

        virtual const string get_usage() const {
            return "echo <message>";
        }

        virtual const string get_help() const {
            return "Prints <message> in your chat window. Color tags are enabled.\nFor more info see \"list\"";
        }

        virtual PLUGIN_RESULT invoke(edict_t *pEdict, const string &args, const vector<string> &argv) const {
            if(argv.size() < 2) {
                console::println(pEdict, "Missing arg");
                return PLUGIN_STOP;
            }
            string parsed(args);
            if (!chatcolor::parse_colors(parsed))
                console::println(pEdict, "Message contains bad tags");
            chat::say(pEdict, parsed);
            return PLUGIN_STOP;
        }
    };

    class SayCommand : public ColorCommand {
    public:
        virtual const string get_name() const {
            return "say";
        }

        virtual const string get_description() const {
            return "Chat in color";
        }

        virtual const string get_usage() const {
            return "say <message>";
        }

        virtual const string get_help() const {
            return "Sends <message> to everyone in the server. Color tags are enabled.\nFor more info see \"list\"";
        }

        virtual PLUGIN_RESULT invoke(edict_t *pEdict, const string &args, const vector<string> &argv) const {
            if(argv.size() < 2) {
                console::println(pEdict, "Missing arg");
                return PLUGIN_STOP;
            }
            string parsed(args);
            if (!chatcolor::parse_colors(parsed))
                console::println(pEdict, "Message contains bad tags");
            chat::say_all(parsed);
            return PLUGIN_STOP;
        }
    };

    class SayTeamCommand : public ColorCommand {
        virtual const string get_name() const {
            return "say_team";
        }

        virtual const string get_description() const {
            return "Teamchat in color";
        }

        virtual const string get_usage() const {
            return "say_team <message>";
        }

        virtual const string get_help() const {
            return "Sends <message> to everyone on your team. Color tags are enabled.\nFor more info see \"list\"";
        }

        virtual PLUGIN_RESULT invoke(edict_t *pEdict, const string &args, const vector<string> &argv) const {
            if(argv.size() < 2) {
                console::println(pEdict, "Missing arg");
                return PLUGIN_STOP;
            }
            string parsed(args);
            if (!chatcolor::parse_colors(parsed))
                console::println(pEdict, "Message contains bad tags");
            chat::say_team(pEdict, parsed);
            return PLUGIN_STOP;
        }
    };

    namespace colorcommands {

        void register_commands() {
            for (auto cc :  initializer_list<ColorCommand *>({
                        new HelpCommand(), new VersionCommand(),
                        new AvailableColorsCommand(), new EchoCommand(),
                        new SayCommand(), new SayTeamCommand()}))
                _commands[cc->get_name()] = cc;
        }

    }

}
