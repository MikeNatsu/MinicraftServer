#ifndef MINICRAFTSERVER_SOCKET_H
#define MINICRAFTSERVER_SOCKET_H

#include <cstdint>
#include <memory>
#include <string>

#include <arpa/inet.h>

namespace mcplus::utils {

    class SocketServer;
    class Socket;

    class SocketServer {
        std::uint32_t sock;
        sockaddr_in serv_addr{};

        std::uint16_t port;
        std::uint32_t listenSock;

        void bindConnection();
    public:
        SocketServer(std::uint16_t port, std::uint32_t listen);
        ~SocketServer();

        [[nodiscard]] std::shared_ptr<Socket> acceptSock() const;
    };

    class Socket {
        int sock;
        std::string IP;
        std::uint16_t port;

        mutable bool connected;

        void bindConnection();
    public:
        Socket(const std::string& IP, std::uint16_t port);
        explicit Socket(int sock);
        ~Socket();

        const std::string& getIP() const;
        std::uint16_t getPort() const;

        void write(std::uint8_t byte) const;
        void write(std::unique_ptr<std::uint8_t[]> bytes, std::size_t len) const;

        std::uint8_t read() const;
        std::unique_ptr<std::uint8_t[]> read(std::size_t len) const;

        bool isConnected() const;
        void close();
    };

    template<typename T>
    void writeNumber(const Socket& socket, T number) {
        std::size_t size = sizeof(T);
        std::unique_ptr<uint8_t[]> bytes = std::make_unique<uint8_t[]>(size);
        for (std::size_t i = 0; i < size; i++) {
            bytes[i] = (number >> (i * 8)) & 0xFF;
        }
        socket.write(std::move(bytes), size);
    }

    template<typename T>
    T readNumber(const Socket& socket) {
        std::size_t size = sizeof(T);
        std::unique_ptr<uint8_t[]> bytes = socket.read(size);

        T value = 0;
        for (std::size_t i = 0; i < size; i++) {
            value |= (bytes[i] & 0xFF) << (i * 8);
        }

        return value;
    }

    void writeString(const Socket& socket, const std::string& string);
    std::string readString(const Socket& socket);

    void writeLegacyString(const Socket& socket, const std::string& string);
    std::string readLegacyString(const Socket& socket);

}

#endif //MINICRAFTSERVER_SOCKET_H
