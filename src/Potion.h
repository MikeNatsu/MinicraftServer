#ifndef MINICRAFTSERVER_POTION_H
#define MINICRAFTSERVER_POTION_H

#include <cstdint>
#include <string>

namespace mcplus {
    enum class PotionType {
        NONE,
        SPEED,
        LIGHT,
        SWIM,
        ENERGY,
        REGEN,
        HEALTH,
        TIME,
        LAVA,
        SHIELD,
        HASTE,
        ESCAPE
    };

    struct Potion {
        PotionType type;
        int32_t duration; // duration measured in seconds

        Potion(PotionType type, int32_t duration);
        explicit Potion(PotionType type);
        Potion();

        bool operator==(const Potion& potion) const;
    };

    int32_t getPotionDuration(PotionType type);

    std::string getPotionName(PotionType type);
    PotionType getPotionType(const std::string& potionName);
}

#endif // MINICRAFTSERVER_POTION_H