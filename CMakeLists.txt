cmake_minimum_required(VERSION 3.17)
project(MinicraftServer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS -O2)

include_directories(src)
add_library(MinicraftLib src/Dimension.h src/Dimension.cpp src/Server.cpp src/Server.h src/World.h src/World.cpp src/MinicraftDef.h src/MinicraftDef.cpp src/Entity.h src/Socket.h src/Socket.cpp src/Protocol.h src/Protocol.cpp src/Packet.h src/Packet.cpp src/Inventory.h src/Inventory.cpp src/Potion.h src/Potion.cpp src/Utils.h src/Utils.cpp src/Entity.cpp)
add_executable(MinicraftServer src/main.cpp)
target_link_libraries(MinicraftServer MinicraftLib -lpthread)
