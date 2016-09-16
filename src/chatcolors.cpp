#include "chatcolors.h"
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
        bool parse_colors(string &str, bool strip) {
            if(_name_to_id.empty()) {
                // Init _name_to_id
                for(ID id = chatcolor::min; id <= chatcolor::max; id++) {
                    string name(chatcolor::name(id));
                    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
                    _name_to_id[name] = id;
                }
            }

            // some cool text{#color_name_or_value}some colored text
            //        start--^ ^                  ^^--end
            //                 |                  |
            //            start_name           end_name
            size_t pos = 0, pos_value, start, start_name, end, end_name, len, len_name;
            //size_t total_offset = 0;
            int value;
            bool success = true;

            //INFO("Parsing colored message \"%s\"", str.c_str());
            while (true) {
                start = str.find("{#", pos);
                if(start == string::npos) {
                    break;
                }
                start_name = start + 2;
                end_name = str.find("}", start_name);
                if(end_name == string::npos) {
                    success = false;
                    //WARN("\tExpected \"}\" after %d", start_name + total_offset);
                    pos = start_name;
                    continue;
                }
                end = end_name + 1;
                len_name = end_name - start_name;
                len = end - start;
                pos = end;
                if(len_name == 0) {
                    success = false;
                    //WARN("\tEmpty color tag at %d", start_name + total_offset);
                    continue;
                }
                string name = str.substr(start_name, len_name);
                //INFO("\tParsing \"%s\" ", name.c_str());
                if(isdigit(name[0])) {
                    try {
                        value = stoi(name, &pos_value);
                    } catch(const invalid_argument &ex) {
                        success = false;
                        //WARN("\t\tNon integer value at %d", start_name + total_offset);
                        continue;
                    } catch(const out_of_range &ex) {
                        success = false;
                        //WARN("\t\tValue out of range at %d", start_name + total_offset);
                        continue;
                    }
                    if(len_name != pos_value) {
                        success = false;
                        //WARN("\t\tNon integer value at %d", start_name + total_offset);
                        continue;
                    }
                    if(value < chatcolor::min || value > chatcolor::max) {
                        success = false;
                        //WARN("\t\tValue out of range at %d", start_name + total_offset);
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
                        //WARN("\t\tUnknown name or missing \"}\" after %d", start_name + total_offset);
                        pos = start_name;
                        continue;
                    }
                }
                //total_offset += len;
                pos = start;
                if(!strip) {
                    //total_offset--;
                    pos++;
                    if(start == 0) {
                        //INFO("\t\tInserting space at beginning of string");
                        str.insert(0, " ");
                        pos++;
                    }
                }
                //INFO("\t\tParse successfull");
            }
            return success;
        }

    }
}
