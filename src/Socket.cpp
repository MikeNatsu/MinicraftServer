#include "Socket.h"

#include <arpa/inet.h>
#include <sys/socket.h>

#include <stdexcept>

using namespace mcplus::utils;

SocketServer::SocketServer(std::uint16_t port, std::uint32_t listen) {
    this->sock       = 0;
    this->port       = port;
    this->listenSock = listen;

    try {
        bindConnection();
    } catch (const std::exception& e) {
        if (this->sock != 0) {
            shutdown(this->sock, SHUT_RDWR);
        }
        throw;
    }
}

SocketServer::~SocketServer() {
    if (this->sock != 0) {
        shutdown(this->sock, SHUT_RDWR);
    }
}

void SocketServer::bindConnection() {
    this->sock = socket(AF_INET, SOCK_STREAM, 0);
    this->serv_addr = {};

    if (this->sock == 0) {
        throw std::logic_error("Error to create socket");
    }

    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port        = htons(this->port);

    if (bind(sock, (sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        throw std::logic_error("Error to bind");
    }
    if (listen(sock, listenSock) < 0) {
        throw std::logic_error("Error to listenSock");
    }
}

std::shared_ptr<Socket> SocketServer::acceptSock() const {
    std::size_t addr_length = sizeof(serv_addr);
    int sock_f = accept(this->sock, (sockaddr *) &serv_addr, (socklen_t *) &addr_length);
    if (sock_f < 0) {
        throw std::logic_error("Error to accept");
    }

    return std::make_shared<Socket>(sock_f);
}

Socket::Socket(const std::string& IP, std::uint16_t port) {
    this->sock = 0;
    this->IP   = IP;
    this->port = port;
    this->connected = false;

    try {
        bindConnection();
    } catch (const std::exception& e) {
        if (this->sock != 0) {
            shutdown(this->sock, SHUT_RDWR);
        }
        throw;
    }
}

Socket::Socket(int sock) {
    this->sock = sock;

    sockaddr_in serv_addr{};
    socklen_t len = sizeof(serv_addr);
    getpeername(sock, (sockaddr *) &serv_addr, &len);

    this->IP = inet_ntoa(serv_addr.sin_addr);
    this->port = ntohs(serv_addr.sin_port);

    this->connected = true;
}

Socket::~Socket() {
    close();
}

void Socket::bindConnection() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        throw std::logic_error("Error to create socket");
    }

    sockaddr_in serv_addr = {.sin_family = AF_INET, .sin_port = htons(port)};
    if (inet_pton(AF_INET, IP.c_str(), &serv_addr.sin_addr) <= 0) {
        throw std::logic_error("Invalid address");
    }
    if (connect(sock, (sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        throw std::logic_error("It couldn't connect to server");
    }

    this->connected = true;
}

const std::string& Socket::getIP() const {
    return this->IP;
}

std::uint16_t Socket::getPort() const {
    return this->port;
}

void Socket::write(uint8_t byte) const {
    std::size_t result = send(sock, &byte, sizeof(uint8_t), 0);
    if (result == -1) {
        this->connected = false;
        throw std::logic_error("Error to send sock");
    }
}

void Socket::write(std::unique_ptr<std::uint8_t[]> bytes, std::size_t len) const {
    std::size_t left = len;

    // make sure to send the whole data
    while (left > 0) {
        int result = send(sock, bytes.get() + (len - left), sizeof(uint8_t) * left, 0);
        if (result == -1) {
            this->connected = false;
            throw std::logic_error("Error to send sock");
        }
        left -= result;
    }
}

uint8_t Socket::read() const {
    uint8_t byte = 0;
    int result = recv(sock, &byte, sizeof(uint8_t), 0);
    if (result == -1) {
        this->connected = false;
        throw std::logic_error("Socket::read(): Error to receive sock");
    }
    return byte;
}

std::unique_ptr<uint8_t[]> Socket::read(std::size_t len) const {
    std::unique_ptr<uint8_t[]> bytes = std::make_unique<uint8_t[]>(len);

    std::size_t left = len;
    while (left > 0) {
        int result = recv(sock, bytes.get() + (len - left), sizeof(uint8_t) * left, 0);
        if (result == -1) {
            this->connected = false;
            throw std::logic_error("Error to receive sock");
        }
        left -= result;
    }

    return std::move<>(bytes);
}

bool Socket::isConnected() const {
    return this->connected;
}

void Socket::close() {
    if (this->sock != 0) {
        shutdown(this->sock, SHUT_RDWR);
    }
    this->connected = false;
    this->sock = 0;
}

void mcplus::utils::writeString(const Socket& socket, const std::string& string) {
    std::size_t length = string.size();
    std::unique_ptr<uint8_t[]> bytes = std::make_unique<uint8_t[]>(length);

    for (std::size_t i = 0; i < length; i++) {
        bytes[i] = string[i];
    }

    writeNumber<std::size_t>(socket, length);
    socket.write(std::move(bytes), length);
}

std::string mcplus::utils::readString(const Socket& socket) {
    auto length = readNumber<std::size_t>(socket);
    std::unique_ptr<uint8_t[]> bytes = socket.read(length);
    return std::string(reinterpret_cast<char *>(bytes.get()), length);
}

void mcplus::utils::writeLegacyString(const Socket& socket, const std::string& string) {
    std::size_t length = string.size() + 1;
    std::unique_ptr<uint8_t[]> bytes = std::make_unique<uint8_t[]>(length);

    for (std::size_t i = 0; i < length; i++) {
        bytes[i] = i + 1 < length ? string[i] : '\0';
    }

    socket.write(std::move(bytes), length);
}

std::string mcplus::utils::readLegacyString(const Socket& socket) {
    std::string string{};
    for (std::uint8_t ch = socket.read(); ch != '\0'; ch = socket.read()) {
        string += (char) ch;
    }
    return string;
}
