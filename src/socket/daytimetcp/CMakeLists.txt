cmake_minimum_required(VERSION 2.6)
project (Socket)

set (Socket_VERSION_MAJOR 1)
set (Socket_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_definitions(-Wall)
add_definitions(-g)

add_executable(Client daytimetcp_client.cpp)
add_executable(Server daytimetcp_server.cpp)