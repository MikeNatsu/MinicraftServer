#include "Protocol.h"

using namespace mcplus;

void mcplus::writePacket(utils::Socket& socket, const Packet& packet) {
    RawPacket rawPacket = (RawPacket) packet;

    socket.write(rawPacket.id);
    utils::writeLegacyString(socket, rawPacket.data);
}

RawPacket mcplus::readPacket(utils::Socket& socket) {
    return {socket.read(), utils::readLegacyString(socket)};
}