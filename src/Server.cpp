#include "Server.h"
#include "Packet.h"
#include "Utils.h"

#include <iostream>
#include <thread>
#include <utility>
#include <chrono>

using namespace mcplus;

static bool defaultPacketHandler(utils::Socket& client, const RawPacket& rawPacket);
static std::unordered_map<std::string, std::shared_ptr<CommandExecutor>> defaultCommandMap();

class CommandSender : public Sender {
public:
    CommandSender() = default;

    void sendMessage(const std::string& message) const override;
};

void CommandSender::sendMessage(const std::string& message) const {
    std::cout << message << std::endl;
}

PlayerSocket::PlayerSocket(std::shared_ptr<utils::Socket> socket) {
    this->socket = std::move(socket);
    this->thread = nullptr;
    this->packetHandler = defaultPacketHandler;
}

PlayerSocket::~PlayerSocket() {
    if (thread == nullptr) {
        return;
    }

    thread->detach();
    delete thread;
}

void PlayerSocket::run() {
    int bad_packets = 0;
    while (this->isConnected()) {
        try {
            if (packetHandler(*socket, readPacket(*socket))) {
                bad_packets = 0;
            } else {
                bad_packets++;
            }
        } catch (const std::logic_error& exception) {
            std::cerr << socket->getIP() << ':' << socket->getPort() << " got disconnected " << exception.what() << std::endl;
        }

        if (bad_packets > 15) {
            writePacket(*socket, InvalidPacket("Many bad packets"));
            socket->close();
        }
    }
}

void PlayerSocket::start() {
    if (thread != nullptr) {
        return;
    }

    thread = new std::thread([this](){this->run();});
}

bool PlayerSocket::isConnected() const {
    return socket->isConnected();
}

Server::Server(const std::string &ip, short port) {
    this->socketServer = std::make_unique<utils::SocketServer>(port, 100);
    this->running = false;

    this->worldMap  = {};
    this->socketList = {};
    this->listenerList = {};
    this->commandMap = std::move(defaultCommandMap());
}

bool Server::dispatchCommand(const std::string& command) {
    std::vector<std::string> arguments{};
    arguments.reserve(16);
    utils::splitString(command, " ", arguments);

    auto it = commandMap.find(utils::toLower(arguments[0]));
    if (it != commandMap.end()) {
        arguments.erase(arguments.begin());
        it->second->execute(*this, CommandSender(), arguments);

        return true;
    }

    return false;
}

void Server::run() {
    running = true;

    std::thread joinerThread([&]() {
        std::cout << "Connection thread started\n";
        while (running) {
            try {
                std::shared_ptr<utils::Socket> socket = socketServer->acceptSock();
                std::cout << "Connected socket! " << socket->getIP() << ":" << socket->getPort() << "\n";

                socketList.emplace_back(socket);
                socketList[socketList.size() - 1].start();
            } catch (const std::logic_error& exception) {
                if (running) {
                    throw exception;
                }
            }
        }
    });

    std::cout << "Main thread started\n";

    auto oldClock = std::chrono::high_resolution_clock::now();
    auto tps = 1000000000.0f / 60.0f;
    double delta = 0;

    int ticks = 0;

    auto timer = std::chrono::high_resolution_clock::now();

    while (running) {
        auto nowClock = std::chrono::high_resolution_clock::now();
        delta += (nowClock - oldClock).count() / tps;
        oldClock = nowClock;

        while (delta > 0) {
            ticks++;

            delta--;
        }

        // take a mini break :]
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        if ((std::chrono::high_resolution_clock::now() - timer).count() >= 1000000000) {
            ticks = 0;
            timer = std::chrono::high_resolution_clock::now();
        }
    }

    joinerThread.detach();
}

bool Server::isShutdown() const {
    return !running;
}

void Server::shutdown() {
    if (!running) {
        return;
    }

    running = false;
    std::cout << "Shutdown!\n";
}

static bool defaultPacketHandler(utils::Socket& client, const RawPacket& rawPacket) {
    switch (static_cast<PacketType>(rawPacket.id)) {
        case PacketType::INVALID:
            return true;
        case PacketType::PING: {
            std::cout << client.getIP() << ":" << client.getPort() << " pong!" << std::endl;
            return true;
        }
        case PacketType::USERNAMES:
            return false;
        case PacketType::LOGIN: {
            LoginPacket login{rawPacket};
            std::cout << "Username: " << login.username << " - Version: " << (std::string) login.version << std::endl;
            writePacket(client, PlayerPacket{login.version, 0, 0, 0, 0, 10, 10, 0, 0, ItemMaterial::NULL_MATERIAL, 0, 0, {}, {}, false, {}});
            writePacket(client, InitPacket{12, 128, 128, 0, 0, 0});

            return true;
        }
        case PacketType::GAME:
            return false;
        case PacketType::INIT:
            return true;
        case PacketType::LOAD: {
            writePacket(client, TilesPacket{std::vector<Tile>(128*128)});
            writePacket(client, EntitiesPacket{std::vector<std::shared_ptr<Entity>>{}});
            writePacket(client, GamePacket("survival", 6000, 1, true, 10, 1, 1));

            return true;
        }
        case PacketType::TILES:
            return false;
        case PacketType::ENTITIES:
            return false;
        case PacketType::TILE:
            return false;
        case PacketType::ENTITY:
            return false;
        case PacketType::PLAYER:
            return false;
        case PacketType::MOVE: {
            MovePacket move{rawPacket};

            return true;
        }
        case PacketType::ADD:
            return false;
        case PacketType::REMOVE:
            return false;
        case PacketType::DISCONNECT: {
            client.close();
            return true;
        }
        case PacketType::SAVE:
            return true;
        case PacketType::NOTIFY:
            return false;
        case PacketType::INTERACT:
            return true;
        case PacketType::PUSH:
            return true;
        case PacketType::PICKUP:
            return true;
        case PacketType::CHEST_IN:
            return true;
        case PacketType::CHEST_OUT:
            return true;
        case PacketType::ADD_ITEMS:
            return false;
        case PacketType::BED:
            return true;
        case PacketType::POTION:
            return true;
        case PacketType::HURT:
            return false;
        case PacketType::DIE:
            return true;
        case PacketType::RESPAWN:
            return true;
        case PacketType::DROP:
            return true;
        case PacketType::STAMINA:
            return false;
        case PacketType::SHIRT:
            return true;
        case PacketType::STOPFISHING:
            return false;
    }

    return false;
}

static std::unordered_map<std::string, std::shared_ptr<CommandExecutor>> defaultCommandMap() {
    class StopCommand : public CommandExecutor {
    public:
        void execute(IServer& server, const mcplus::Sender& sender, const std::vector<std::string>& args) override {
            sender.sendMessage("Stopping server");
            server.shutdown();
        }
    };
    class PingCommand : public CommandExecutor {
    public:
        void execute(IServer& server, const mcplus::Sender& sender, const std::vector<std::string>& args) override {
            sender.sendMessage("Not implemented yet :(");
        }
    };

    static std::unordered_map<std::string, std::shared_ptr<CommandExecutor>> _data{
        {"stop", std::make_shared<StopCommand>()},
        {"ping", std::make_shared<PingCommand>()}
    };

    return _data;
}
