#ifndef MINICRAFTSERVER_MINICRAFTDEF_H
#define MINICRAFTSERVER_MINICRAFTDEF_H

#include <cstdint>
#include <string>
#include <vector>

namespace mcplus {
    using WorldId   = std::int16_t;
    using WorldSeed = std::int64_t;
    using TileId    = std::uint16_t;

    using ItemId = std::uint16_t;

    using EntityId = std::uint32_t;
    using PlayerId = mcplus::EntityId;

    using PacketId = std::uint16_t;

    using Damage_t = float;

    struct VersionPack {
        int version;
        int major;
        int minor;

        explicit VersionPack(const std::string& stringVersion);
        VersionPack(int version, int major, int minor);
        VersionPack();

        VersionPack& operator=(const VersionPack& anotherVersion);
        VersionPack& operator=(const std::string& stringVersion);

        explicit operator std::string() const;
    };

    struct Color {
        int8_t alpha;
        int8_t red;
        int8_t green;
        int8_t blue;

        Color(int8_t alpha, int8_t red, int8_t green, int8_t blue);
        explicit Color(int32_t raw);
        Color();

        [[nodiscard]] int32_t raw() const;
    };

    struct FixedLocation {
        int32_t x         : 28;
        uint32_t xDecimal : 4;
        int32_t y         : 28;
        uint32_t yDecimal : 4;

        FixedLocation(int32_t x, uint32_t xDecimal, int32_t y, uint32_t yDecimal);
        FixedLocation(int32_t packed_x, int32_t packed_y);
        FixedLocation();

        FixedLocation operator+(const FixedLocation& another) const;
    };

    class Sender {
    public:
        virtual void sendMessage(const std::string& message) const = 0;
    };

    class IServer {
    public:
        [[nodiscard]] virtual bool isShutdown() const = 0;
        virtual void shutdown() = 0;
    };

    class CommandExecutor {
    public:
        virtual void execute(IServer& server, const Sender& sender, const std::vector<std::string>& args) = 0;
    };

}

#endif //MINICRAFTSERVER_MINICRAFTDEF_H
