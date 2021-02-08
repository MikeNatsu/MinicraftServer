#ifndef MINICRAFTSERVER_SERVER_H
#define MINICRAFTSERVER_SERVER_H

#include <string>
#include <thread>
#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>

#include "MinicraftDef.h"
#include "Socket.h"
#include "World.h"
#include "Protocol.h"
#include "Event.h"

namespace mcplus {

    class PlayerSocket {
        std::thread* thread;
        PacketHandler packetHandler;
    public:
        std::shared_ptr<utils::Socket> socket;

        explicit PlayerSocket(std::shared_ptr<utils::Socket> socket);
        ~PlayerSocket();

        virtual void run();

        void start();
        [[nodiscard]] bool isConnected() const;
    };

    class Server : public IServer {
        std::unique_ptr<utils::SocketServer> socketServer;
        bool running;

        std::unordered_map<WorldId, World> worldMap;
        std::vector<PlayerSocket> socketList;
        std::vector<EventListener> listenerList;
        std::unordered_map<std::string, std::shared_ptr<CommandExecutor>> commandMap;
    public:
        Server(const std::string& ip, short port);

        std::vector<const World&> getLoadedWorld() const;
        std::vector<World&> getLoadedWorld();

        const World& getWorld(WorldId id) const;
        World& getWorld(WorldId id);

        void loadWorld(const std::string& worldName);
        void unloadWorld(WorldId id);

        bool dispatchCommand(const std::string& command);

        void run();

        bool isShutdown() const override;
        void shutdown() override;
    };

}

#endif // MINICRAFTSERVER_SERVER_H
