#include <iostream>
#include <thread>
#include <memory>

#include "Server.h"

int main() {
    std::unique_ptr<mcplus::Server> server = std::make_unique<mcplus::Server>("127.0.0.1", 4225);

    std::thread consoleReader{[&server]() {
        while (!server->isShutdown()) {
            std::string line{};
            std::getline(std::cin, line);

            std::cout << "Server issued: '" << line << "'" << std::endl;
            if (!server->dispatchCommand(line)) {
                std::cout << "Command '" << line << "' doesn't exist" << std::endl;
            }
        }
    }};

    server->run();
    consoleReader.join();

    return 0;
}
