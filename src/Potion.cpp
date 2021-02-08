#include "Potion.h"

#include <unordered_map>

using namespace mcplus;

Potion::Potion(PotionType type, int32_t duration) {
    this->type     = type;
    this->duration = duration;
}

Potion::Potion(PotionType type) {
    this->type     = type;
    this->duration = getPotionDuration(type);
}

Potion::Potion() {
    this->type     = PotionType::NONE;
    this->duration = 0;
}

bool Potion::operator==(const Potion& potion) const {
    return type == potion.type && duration == potion.duration;
}

int32_t mcplus::getPotionDuration(PotionType type) {
    static std::unordered_map<PotionType, int32_t> _data{
        {PotionType::NONE,     0},
        {PotionType::SPEED,   70},
        {PotionType::LIGHT,  100},
        {PotionType::SWIM,    80},
        {PotionType::ENERGY, 140},
        {PotionType::REGEN,   30},
        {PotionType::HEALTH,   0},
        {PotionType::TIME,    30},
        {PotionType::LAVA,   120},
        {PotionType::SHIELD,  90},
        {PotionType::HASTE,   80},
        {PotionType::ESCAPE,   0}
    };

    return _data[type];
}

std::string mcplus::getPotionName(PotionType type) {
    static std::unordered_map<PotionType, std::string> _data{
        {PotionType::NONE,   "None"},
        {PotionType::SPEED,  "Speed"},
        {PotionType::LIGHT,  "Light"},
        {PotionType::SWIM,   "Swim"},
        {PotionType::ENERGY, "Energy "},
        {PotionType::REGEN,  "Regen"},
        {PotionType::HEALTH, "Health"},
        {PotionType::TIME,   "Time"},
        {PotionType::LAVA,   "Lava"},
        {PotionType::SHIELD, "Shield"},
        {PotionType::HASTE,  "Haste"},
        {PotionType::ESCAPE, "Escape"}
    };

    return _data[type];
}

PotionType mcplus::getPotionType(const std::string& potionName) {
    static std::unordered_map<std::string, PotionType> _data{
        {"None",        PotionType::NONE},
        {"Speed",  PotionType::SPEED},
        {"Light",  PotionType::LIGHT},
        {"Swim",   PotionType::SWIM},
        {"Energy", PotionType::ENERGY},
        {"Regen",  PotionType::REGEN},
        {"Health", PotionType::HEALTH},
        {"Time",   PotionType::TIME},
        {"Lava",   PotionType::LAVA},
        {"Shield", PotionType::SHIELD},
        {"Haste",  PotionType::HASTE},
        {"Escape", PotionType::ESCAPE}
    };

    return _data[potionName];
}

