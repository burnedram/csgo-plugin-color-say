#include "chatcolors.h"
#include <vector>

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

        const std::array<const char *, count> names = {
            "White",                "Red",                  "Lavender",             "Green",
            "Pale green",           "Green yellow",         "Coral red",            "Light grey",
            "Light Goldenrod",      "Echo blue",            "Picton blue",          "Royal blue",
            "Purple",               "Neon purple",          "Vermilion",            "Orange gold"
        };

        const std::array<const RGB, count> rgbs = {
            255, 255, 255,          238,   2,   2,          185, 129, 239,           64, 254,  64,
            190, 254, 143,          161, 254,  71,          254,  64,  64,          196, 201, 207,
            236, 227, 122,          174, 193, 215,           94, 151, 216,           75, 105, 254,
            136,  71, 254,          210,  44, 229,          234,  75,  75,          225, 172,  57
        };

    }
}
