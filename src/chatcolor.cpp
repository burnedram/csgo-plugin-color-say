#include "chatcolor.h"
#include "utils.h"
#include <array>
#include <stdexcept>
#include <algorithm>
#include <unordered_map>

using namespace std;

namespace colorsay {
    namespace chatcolor {

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

        const std::array<const char *, count> names = {{
            "White",                "Red",                  "Lavender",             "Green",
            "Pale green",           "Green yellow",         "Coral red",            "Light grey",
            "Light Goldenrod",      "Echo blue",            "Picton blue",          "Royal blue",
            "Purple",               "Neon purple",          "Vermilion",            "Orange gold"
        }};

        const std::array<const RGB, count> rgbs = {{
           {255, 255, 255},        {238,   2,   2},        {185, 129, 239},        { 64, 254,  64},
           {190, 254, 143},        {161, 254,  71},        {254,  64,  64},        {196, 201, 207},
           {236, 227, 122},        {174, 193, 215},        { 94, 151, 216},        { 75, 105, 254},
           {136,  71, 254},        {210,  44, 229},        {234,  75,  75},        {225, 172,  57}
        }};

        static unordered_map<string, ID> _name_to_id;
        void init_color_parser() {
            for(ID id = chatcolor::min; id <= chatcolor::max; id++) {
                string name(chatcolor::name(id));
                std::transform(name.begin(), name.end(), name.begin(), ::tolower);
                _name_to_id[name] = id;
            }
        }

        /*
         * Parses color tags to their char value, i.e:
         * This text is{#red} red -> This text is\x02 red
         * If strip is set to true the tags are removed instead, i.e:
         * This text is{#red} red -> This text is red
         *
         * A color tag is in the form of "{#color_name_or_value}".
         * Color value examples: "{#2}", "{#13}"
         * All color names can be found in the colorsay::chatcolor::tags array.
         * Also see colorsay::chatcolor::tag
         */
        static bool __parse_colors(string &str, bool strip) {
            // some cool text{#color_name_or_value}some colored text
            //        start--^ ^                  ^^--end
            //                 |                  |
            //            start_name           end_name
            size_t pos = 0, pos_value, start, start_name, end, end_name, len, len_name;
            int value;
            bool success = true;

            while (true) {
                start = str.find("{#", pos);
                if(start == string::npos) {
                    break;
                }
                start_name = start + 2;
                end_name = str.find("}", start_name);
                if(end_name == string::npos) {
                    success = false;
                    pos = start_name;
                    continue;
                }
                end = end_name + 1;
                len_name = end_name - start_name;
                len = end - start;
                pos = end;
                if(len_name == 0) {
                    success = false;
                    continue;
                }
                string name = str.substr(start_name, len_name);
                if(isdigit(name[0])) {
                    try {
                        value = stoi(name, &pos_value);
                    } catch(const invalid_argument &ex) {
                        success = false;
                        continue;
                    } catch(const out_of_range &ex) {
                        success = false;
                        continue;
                    }
                    if(len_name != pos_value) {
                        success = false;
                        continue;
                    }
                    if(value < chatcolor::min || value > chatcolor::max) {
                        success = false;
                        continue;
                    }
                    if(strip) {
                        str.erase(start, len);
                    } else {
                        str.replace(start, len, " ");
                        str[start] = static_cast<char>(value);
                    }
                } else {
                    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
                    auto id = _name_to_id.find(name);
                    if (id != _name_to_id.end()) {
                        if(strip) {
                            str.erase(start, len);
                        } else {
                            str.replace(start, len, " ");
                            str[start] = id->second;
                        }
                    } else {
                        success = false;
                        pos = start_name;
                        continue;
                    }
                }
                pos = start;
                if(!strip) {
                    pos++;
                    if(start == 0 && value != WHITE) {
                        // We need a valid character at the start of the message
                        // If the color is white however we don't have to do anything
                        str.insert(0, " ");
                        pos++;
                    }
                }
            }
            return success;
        }

        bool parse_colors(std::string &str) {
            return __parse_colors(str, false);
        }

        bool strip_colors(std::string &str) {
            return __parse_colors(str, true);
        }

    }
}
