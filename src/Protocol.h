#ifndef MCPLUS_PROTOCOL_HEADER
#define MCPLUS_PROTOCOL_HEADER

#include <cstdint>
#include <string>
#include <functional>

#include "MinicraftDef.h"
#include "Socket.h"

namespace mcplus {

    // the minecraft plus protocol
    enum class PacketType : PacketId {
        INVALID     = 0x01,
        PING        = 0x02,
        USERNAMES   = 0x03, // actually it's not used
        LOGIN       = 0x04,
        GAME        = 0x05,
        INIT        = 0x06,
        LOAD        = 0x07,
        TILES       = 0x08,
        ENTITIES    = 0x09,
        TILE        = 0x0A,
        ENTITY      = 0x0B,
        PLAYER      = 0x0C,
        MOVE        = 0x0D,
        ADD         = 0x0E,
        REMOVE      = 0x0F,
        DISCONNECT  = 0x10,
        SAVE        = 0x11,
        NOTIFY      = 0x12,
        INTERACT    = 0x13,
        PUSH        = 0x14,
        PICKUP      = 0x15,
        CHEST_IN    = 0x16,
        CHEST_OUT   = 0x17,
        ADD_ITEMS   = 0x18,
        BED         = 0x19,
        POTION      = 0x1A,
        HURT        = 0x1B,
        DIE         = 0x1C,
        RESPAWN     = 0x1D,
        DROP        = 0x1E,
        STAMINA     = 0x1F,
        SHIRT       = 0x20,
        STOPFISHING = 0x21
    };

    struct RawPacket {
        PacketId id;
        std::string data;
    };

    struct Packet {
        virtual explicit operator RawPacket() const = 0;
        virtual Packet& operator=(const RawPacket& raw) = 0;
    };

    using PacketHandler = std::function<bool(utils::Socket&, const RawPacket&)>;

    void writePacket(utils::Socket& socket, const Packet& packet);
    RawPacket readPacket(utils::Socket& socket);

}

#endif // MCPLUS_PROTOCOL_HEADER
