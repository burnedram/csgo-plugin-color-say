#pragma once
#include <cstdint>
#include <array>
#include <ostream>
#include <string>

namespace colorsay {
    namespace chatcolor {

        enum ID : char {
            WHITE = 1,              RED = 2,                LAVENDER = 3,           GREEN = 4, 
            PALE_GREEN = 5,         GREEN_YELLOW = 6,       CORAL_RED = 7,          LIGHT_GREY = 8,         
            LIGHT_GOLDENROD = 9,    ECHO_BLUE = 10,         PICTON_BLUE = 11,       ROYAL_BLUE = 12,        
            PURPLE = 13,            NEON_PURPLE = 14,       VERMILION = 15,         ORANGE_GOLD = 16
        };

        struct RGB {
            uint8_t r, g, b;
        };

        ID &operator++(ID &id);
        ID operator++(ID &id, int);

        std::ostream &operator<<(std::ostream &out, const ID &id);

        const ID min = static_cast<ID>(1), max = static_cast<ID>(16);
        const size_t count = max - min + 1;
        inline ID random() {
            return static_cast<ID>((rand() % count) + min);
        }

        extern const std::array<const char *, count> names;
        inline const char *name(const ID &id) {
            return names[id - min];
        }

        extern const std::array<const RGB, count> rgbs;
        inline const RGB &rgb(const ID &id) {
            return rgbs[id - min];
        }

        void parse_colors(std::string &str);
        void strip_colors(std::string &str);

    }
}
