#include "Dimension.h"

#include <map>

using namespace mcplus;

Direction mcplus::operator+(const Direction& original, const Direction& direction) {
    return static_cast<Direction>((static_cast<unsigned>(original) + static_cast<unsigned>(direction)) % 5);
}

Direction & mcplus::operator+=(Direction& original, const Direction& direction) {
    original = original + direction;
    return original;
}

Direction mcplus::operator-(const Direction& original, const Direction& direction) {
    return static_cast<Direction>((static_cast<unsigned>(original) - static_cast<unsigned>(direction)) % 5);
}

Direction & mcplus::operator-=(Direction& original, const Direction& direction) {
    original = static_cast<Direction>((static_cast<unsigned>(original) - static_cast<unsigned>(direction)) % 5);
    return original;
}

Direction & mcplus::operator++(Direction& direction) {
    direction = static_cast<Direction>((static_cast<unsigned>(direction) + 1) % 5);
    return direction;
}

Direction & mcplus::operator--(Direction& direction) {
    direction = static_cast<Direction>((static_cast<unsigned>(direction) - 1) % 5);
    return direction;
}

std::ostream& operator<<(std::ostream& ostream, const Direction& direction) {
    static std::map<Direction, const char *> direction_info = {
            {Direction::NONE,  "None"},
            {Direction::DOWN,  "Down"},
            {Direction::UP,    "Up"},
            {Direction::LEFT,  "Left"},
            {Direction::RIGHT, "Right"}
    };

    ostream << direction_info[direction];
    return ostream;
}
