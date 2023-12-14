cmake_minimum_required(VERSION 3.10)

project(CpProxy)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_options(-O0 -g)

set(SOURCES
    src/main.cpp
    src/tcp_server.cpp
    src/connection_bridge.cpp
)

set(HEADERS
    include/tcp_server.hpp
    include/connection_bridge.hpp
)

include_directories(include)

add_executable(cpproxy ${SOURCES} ${HEADERS})

target_include_directories(cpproxy PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
